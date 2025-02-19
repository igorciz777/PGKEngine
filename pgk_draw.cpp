#include "pgk_draw.h"

#include "pgk_core.h"
#include "pgk_math.h"

#include <QPainter>
#include <stack>
#include <QThread>
#include "pgk_raycast.h"

#define COOL_COLOR_EFFECT 0

#if COOL_COLOR_EFFECT == 1
inline void PGK_Draw::drawPixel(QImage &target, const QColor &color, int16_t x0, int16_t y0)
{
    if(x0 < 0 || x0 >= target.width()) return;
    if(y0 < 0 || y0 >= target.height()) return;
    target.bits()[x0 * 4 + y0 * target.bytesPerLine()] = color.blue();
    target.bits()[(x0+1) * 4 + y0 * target.bytesPerLine() + 1] = color.green();
    target.bits()[(x0-1) * 4 + y0 * target.bytesPerLine() + 2] = color.red();
}
#else
inline void PGK_Draw::drawPixel(QImage &target, const QColor &color, int16_t x0, int16_t y0)
{
    if(x0 < 0 || x0 >= target.width()) return;
    if(y0 < 0 || y0 >= target.height()) return;
    const uint32_t* scanLine = reinterpret_cast<const uint32_t*>(target.scanLine(y0));
    uint32_t *pixel = const_cast<uint32_t*>(scanLine + x0);
    *pixel = color.rgba();
}
#endif

inline void PGK_Draw::drawLine(QImage &target, const QColor &color, int16_t x0, int16_t y0, int16_t x1, int16_t y1){
    int16_t dx = std::abs(x1 - x0);
    int8_t sx = x0 < x1 ? 1 : -1;
    int16_t dy = -std::abs(y1 - y0);
    int8_t sy = y0 < y1 ? 1 : -1;
    int32_t error = dx + dy;

    while (x0 != x1 || y0 != y1){
        drawPixel(target, color, x0, y0);
        int32_t error2 = 2 * error;
        if (error2 >= dy){
            error += dy;
            x0 += sx;
        }
        if (error2 <= dx){
            error += dx;
            y0 += sy;
        }
    }
}

inline void PGK_Draw::drawCircle(QImage &target, const QColor &color, int16_t x0, int16_t y0, float radius)
{
    float L = (float)radius/std::sqrt(2);
    for(int x = 0; x <= L; x++){
        int y = std::sqrt((radius*radius)-(x*x));
        drawPixel(target, color, x0+x,y0+y);
        drawPixel(target, color, x0-x,y0+y);
        drawPixel(target, color, x0+x,y0-y);
        drawPixel(target, color, x0-x,y0-y);
        drawPixel(target, color, x0+y,y0+x);
        drawPixel(target, color, x0-y,y0+x);
        drawPixel(target, color, x0+y,y0-x);
        drawPixel(target, color, x0-y,y0-x);
    }
}

void PGK_Draw::drawTriangle(QImage &target, const Triangle &triangle, std::vector<float> &zBuffer, const std::vector<PGK_Light> &lights, const Vec3 &cameraPos, const std::vector<Triangle> &triangleBuffer)
{
    const float area = PGK_Math::edgeFunction(triangle.s0, triangle.s1, triangle.s2);
    if (area <= 0) return; // backface
    const float invArea = 1.0f / area;

    // bounding box
    const int minX = std::clamp(static_cast<int>(std::min({triangle.s0.x, triangle.s1.x, triangle.s2.x})), 0, target.width()-1);
    const int maxX = std::clamp(static_cast<int>(std::max({triangle.s0.x, triangle.s1.x, triangle.s2.x})), 0, target.width()-1);
    const int minY = std::clamp(static_cast<int>(std::min({triangle.s0.y, triangle.s1.y, triangle.s2.y})), 0, target.height()-1);
    const int maxY = std::clamp(static_cast<int>(std::max({triangle.s0.y, triangle.s1.y, triangle.s2.y})), 0, target.height()-1);

    const std::array<float, 3> z = {triangle.ndc0.z, triangle.ndc1.z, triangle.ndc2.z};
    const std::array<Vec2, 3> uv = {triangle.uv0, triangle.uv1, triangle.uv2};
    const std::array<Vec3, 3> norms = {triangle.n0, triangle.n1, triangle.n2};

    const float invW0 = 1.0f / triangle.v0.w;
    const float invW1 = 1.0f / triangle.v1.w;
    const float invW2 = 1.0f / triangle.v2.w;
    const float uv0x = uv[0].x * invW0;
    const float uv0y = uv[0].y * invW0;
    const float uv1x = uv[1].x * invW1;
    const float uv1y = uv[1].y * invW1;
    const float uv2x = uv[2].x * invW2;
    const float uv2y = uv[2].y * invW2;

    const Vec3 viewDir = (cameraPos - triangle.worldPosition).normalize();

    const int texWidth = triangle.material->texture->width();
    const int texHeight = triangle.material->texture->height();

    Vec3 normal; QColor phongColor;
    bool inShadow = false;
    float t;

    if(!g_pgkCore.SMOOTH_SHADING){
        normal = ((norms[0]  + norms[1]  + norms[2])/3).normalize();
        for (const auto& light : lights) {
            inShadow = false;
            const Vec3 surface = triangle.worldPosition + normal * 0.01f;
            const Vec3 lightDir = (light.getLocalPosition() - surface).normalize();

            QColor lightColor = PGK_Draw::calculateFlatLighting(light, lightDir, normal);
            phongColor = QColor(std::min(255, phongColor.red() + lightColor.red()),
                                std::min(255, phongColor.green() + lightColor.green()),
                                std::min(255, phongColor.blue() + lightColor.blue()));

            if (!g_pgkCore.RAYCAST_SHADOWS) continue;
            if (!light.castShadows) continue;
            if (!triangle.receiveShadows) continue;

            for (const auto& shadowCaster : triangleBuffer) {
                if (!shadowCaster.castShadows || shadowCaster == triangle) continue;
                if(triangle.worldPosition.distanceSq(shadowCaster.worldPosition) > g_pgkCore.SHADOW_DRAW_DISTANCE) continue;
                if(inShadow) break;
                if (PGK_Math::intersectTriangle(surface, lightDir, shadowCaster.v0, shadowCaster.v1, shadowCaster.v2, t)) {
                    inShadow = true;
                    break;
                }
            }

            if(inShadow){
                phongColor.setRed(phongColor.red()/2);
                phongColor.setGreen(phongColor.green()/2);
                phongColor.setBlue(phongColor.blue()/2);
            }
        }
    }

    for (int y = minY; y <= maxY; ++y) {
        float* zPtr = &zBuffer[minX + y * target.width()];
        for (int x = minX; x <= maxX; ++x, ++zPtr) {
            if(x < 0 || x >= target.width()) continue;
            if(y < 0 || y >= target.height()) continue;

            const Vec3 p(x + 0.5f, y + 0.5f, 0.f);

            const float w0 = PGK_Math::edgeFunction(triangle.s1,triangle.s2,p);
            const float w1 = PGK_Math::edgeFunction(triangle.s2,triangle.s0,p);
            const float w2 = PGK_Math::edgeFunction(triangle.s0,triangle.s1,p);

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                // barycentryczne
                const float alpha = w0 * invArea;
                const float beta = w1 * invArea;
                const float gamma = w2 * invArea;

                // depth
                float zVal = alpha * z[0] + beta * z[1] + gamma * z[2];


                if (zVal > *zPtr) {
                    *zPtr = zVal;
                    // perspective correction
                    const float w = 1.0f / (alpha * invW0 + beta * invW1 + gamma * invW2);
                    const float u = w * (alpha * uv0x + beta * uv1x + gamma * uv2x);
                    const float v = w * (alpha * uv0y + beta * uv1y + gamma * uv2y);

                    if(g_pgkCore.SMOOTH_SHADING){
                        // interpolacja normali
                        normal = (norms[0] * alpha + norms[1] * beta + norms[2] * gamma).normalize();
                        inShadow = false;
                        phongColor = QColor(0, 0, 0);
                        for (const auto& light : lights) {
                            //barycentric surface
                            const Vec3 surface = Vec3(triangle.v0 * alpha + triangle.v1 * beta + triangle.v2 * gamma);
                            const Vec3 lightDir = (light.getLocalPosition() - surface).normalize();
                            //const Vec3 reflectDir = lightDir.reflect(normal).normalize();

                            QColor lightColor = PGK_Draw::calculateBlinnPhongLighting(light, lightDir,viewDir, normal);
                            phongColor = QColor(std::min(255, phongColor.red() + lightColor.red()),
                                                std::min(255, phongColor.green() + lightColor.green()),
                                                std::min(255, phongColor.blue() + lightColor.blue()));

                            if (!g_pgkCore.RAYCAST_SHADOWS) continue;
                            if (!light.castShadows) continue;
                            if (!triangle.receiveShadows) continue;

                            // Check for intersections with other objects
                            for (const auto& shadowCaster : triangleBuffer) {
                                if (!shadowCaster.castShadows || shadowCaster == triangle) continue;
                                if(triangle.worldPosition.distanceSq(shadowCaster.worldPosition) > g_pgkCore.SHADOW_DRAW_DISTANCE) continue;
                                if(inShadow) break;
                                if (PGK_Math::intersectTriangle(surface, lightDir, shadowCaster.v0, shadowCaster.v1, shadowCaster.v2, t)) {
                                    inShadow = true;
                                    break;
                                }
                            }

                            if(inShadow){
                                phongColor.setRed(phongColor.red()/2);
                                phongColor.setGreen(phongColor.green()/2);
                                phongColor.setBlue(phongColor.blue()/2);
                            }
                        }
                    }

                    // texture sampling
                    QColor texColor;
                    if(g_pgkCore.TEX_FILTERING) texColor = getInterpolatedColor(*triangle.material->texture.get(), u * triangle.material->texture->width(), v * triangle.material->texture->height());
                    else{
                        const int tx = std::clamp(static_cast<int>(u * texWidth), 0, texWidth-1);
                        const int ty = std::clamp(static_cast<int>(v * texHeight), 0, texHeight-1);
                        texColor = getColor(*triangle.material->texture.get(),tx,ty);
                    }

                    QColor finalColor(
                        std::min(255, (phongColor.red() * texColor.red()) / 255),
                        std::min(255, (phongColor.green() * texColor.green()) / 255),
                        std::min(255, (phongColor.blue() * texColor.blue()) / 255)
                        );
                    
                    drawPixel(target, finalColor, x, y);
                }
            }
        }
    }
}

void PGK_Draw::drawText(QImage &target, const QString &text, uint8_t size, int16_t x0, int16_t y0, QColor color)
{
    QPainter painter(&target);
    painter.setPen(color);
    painter.setFont(QFont("Arial", size));
    painter.drawText(x0, y0, text);
    painter.end();
}

inline void PGK_Draw::floodFill(QImage &target, QColor color, QColor newColor, int16_t x0, int16_t y0)
{
    if (color == newColor) return;
    if (x0 < 0 || y0 < 0 || x0 >= target.width() || y0 >= target.height()) return;

    std::stack<QPoint> Q;
    Q.push(QPoint(x0, y0));

    while (!Q.empty()) {
        QPoint cur = Q.top();
        Q.pop();

        int16_t x = cur.x();
        int16_t y = cur.y();

        if (x < 0 || y < 0 || x >= target.width() || y >= target.height()) continue;

        if (target.pixelColor(x, y) != color) continue;

        PGK_Draw::drawPixel(target, newColor, x, y);

        Q.push(QPoint(x + 1, y));
        Q.push(QPoint(x - 1, y));
        Q.push(QPoint(x, y + 1));
        Q.push(QPoint(x, y - 1));
    }
    return;
}

inline void PGK_Draw::scanLine(QImage &target, QColor color, std::vector<QPoint> polygon)
{
    if (polygon.size() < 3) return;

    int16_t ymin = polygon[0].y(), ymax = polygon[0].y();

    for (QPoint point : polygon) {
        ymin = std::min(ymin, (int16_t)point.y());
        ymax = std::max(ymax, (int16_t)point.y());
    }

    for (int16_t y = ymin; y <= ymax; ++y) {
        std::vector<int16_t> intersections;
        for (size_t i = 0; i < polygon.size(); ++i) {
            QPoint p1 = polygon[i];
            QPoint p2 = polygon[(i + 1) % polygon.size()];

            if (p1.y() == p2.y()) continue;
            if (p1.y() > p2.y()) std::swap(p1, p2);

            if (y >= p1.y() && y < p2.y()) {
                float x = p1.x() + (float)(p2.x() - p1.x()) * (y - p1.y()) / (p2.y() - p1.y());
                intersections.push_back((int16_t)floor(x + 0.5));
            }
        }

        std::sort(intersections.begin(), intersections.end());

        for (size_t i = 0; i + 1 < intersections.size(); i += 2) {
            int16_t xStart = intersections[i];
            int16_t xEnd = intersections[i + 1];
            PGK_Draw::drawLine(target, color, xStart, y, xEnd, y);
        }
    }
}

inline QColor PGK_Draw::getColor(const QImage &image, int16_t x, int16_t y)
{
    const uint32_t* scanLine = reinterpret_cast<const uint32_t*>(image.scanLine(y));
    const uint32_t pixel = scanLine[x];
    return QColor(
        (pixel >> 16) & 0xff,
        (pixel >> 8) & 0xff,
        pixel & 0xff
    );
}

inline QColor PGK_Draw::getInterpolatedColor(const QImage &image, float x, float y) {
    const int texWidth = image.width();
    const int texHeight = image.height();

    const int x1 = std::clamp(static_cast<int>(x), 0, texWidth-1);
    const int y1 = std::clamp(static_cast<int>(y), 0, texHeight-1);
    const int x2 = std::clamp(x1 + 1, 0, texWidth-1);
    const int y2 = std::clamp(y1 + 1, 0, texHeight-1);

    const QColor p00 = getColor(image, x1, y1);
    const QColor p10 = getColor(image, x2, y1);
    const QColor p01 = getColor(image, x1, y2);
    const QColor p11 = getColor(image, x2, y2);

    const float a = x - x1;
    const float b = y - y1;

    return PGK_Math::interpolateColor(p00, p10, p01, p11, a, b);
}

inline QColor PGK_Draw::calculatePhongLighting(const PGK_Light &light, const Vec3 &lightDir, const Vec3 &viewDir, const Vec3 &reflectDir, const Vec3 &normal)
{
    const float ambientStrength = .2f;
    const float diffuseStrength = std::max(0.0f, normal.dot(lightDir)) * light.diffusePower;
    const float specularStrength = PGK_Math::fastpow(std::max(0.0f, viewDir.dot(reflectDir)), light.shininess) * light.specularPower;

    const QColor ambient = light.ambientColor;
    const QColor diffuse = light.diffuseColor;
    const QColor specular = light.specularColor;

    const QColor result(
        std::min(255.0f, ambientStrength * ambient.red() + diffuseStrength * diffuse.red() + specularStrength * specular.red()),
        std::min(255.0f, ambientStrength * ambient.green() + diffuseStrength * diffuse.green() + specularStrength * specular.green()),
        std::min(255.0f, ambientStrength * ambient.blue() + diffuseStrength * diffuse.blue() + specularStrength * specular.blue())
        );

    return result;
}

QColor PGK_Draw::calculateBlinnPhongLighting(const PGK_Light &light, const Vec3 &lightDir, const Vec3 &viewDir, const Vec3 &normal)
{
    const float ambientStrength = .2f;
    const float diffuseStrength = std::max(0.0f, normal.dot(lightDir)) * light.diffusePower;
    const Vec3 halfDir = (lightDir + viewDir).normalize();
    const float specularStrength = PGK_Math::fastpow(std::max(0.0f, normal.dot(halfDir)), light.shininess) * light.specularPower;

    const QColor ambient = light.ambientColor;
    const QColor diffuse = light.diffuseColor;
    const QColor specular = light.specularColor; 

    return QColor(
        std::min(255.0f, ambientStrength * ambient.red() + diffuseStrength * diffuse.red() + specularStrength * specular.red()),
        std::min(255.0f, ambientStrength * ambient.green() + diffuseStrength * diffuse.green() + specularStrength * specular.green()),
        std::min(255.0f, ambientStrength * ambient.blue() + diffuseStrength * diffuse.blue() + specularStrength * specular.blue())
        );
}

inline QColor PGK_Draw::calculateFlatLighting(const PGK_Light &light, const Vec3 &lightDir, const Vec3 &normal)
{
    const float ambientStrength = .2f;
    const float diffuseStrength = std::max(0.0f, normal.dot(lightDir)) * light.diffusePower;

    const QColor ambient = light.ambientColor;
    const QColor diffuse = light.diffuseColor;

    return QColor(
        std::min(255.0f, ambientStrength * ambient.red() + diffuseStrength * diffuse.red()),
        std::min(255.0f, ambientStrength * ambient.green() + diffuseStrength * diffuse.green()),
        std::min(255.0f, ambientStrength * ambient.blue() + diffuseStrength * diffuse.blue())
        );
}


