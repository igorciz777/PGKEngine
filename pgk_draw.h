#ifndef PGK_DRAW_H
#define PGK_DRAW_H

#include <QImage>

#include "pgk_light.h"
#include "pgk_math.h"

namespace PGK_Draw
{
    inline void drawPixel(QImage &target, const cVec3 &color, int16_t x0, int16_t y0);
    inline void drawPixel(QImage &target, const cVec3 &color, int16_t x0, int16_t y0);
    inline void drawLine(QImage &target, const cVec3 &color, int16_t x0, int16_t y0, int16_t x1, int16_t y1);
    inline void drawCircle(QImage &target, const cVec3 &color, int16_t x0, int16_t y0, float radius);
    void drawTriangle(QImage &target, const Triangle &triangle, std::vector<float> &zBuffer, const std::vector<std::shared_ptr<PGK_Light>> &lights, const Vec3 &cameraPos, const std::vector<Triangle> &triangleBuffer);
    void drawText(QImage &target, const QString &text, uint8_t size, int16_t x0, int16_t y0, QColor color);

    inline void scanLine(QImage &target, cVec3 color, std::vector<QPoint> polygonPoints);

    inline cVec3 getColor(const QImage &image, int16_t x, int16_t y);
    inline cVec3 getInterpolatedColor(const QImage &image, float x, float y);
    inline cVec3 calculateBlinnPhongLighting(const std::shared_ptr<PGK_Light> &light, Vec3 &lightDir, const Vec3 &viewDir, const Vec3 &normal, const Vec3 &surfacePos);
    inline cVec3 calculateFlatLighting(const std::shared_ptr<PGK_Light> &light, Vec3 &lightDir, const Vec3 &normal, const Vec3 &surfacePos);
    inline cVec3 calculateGGXLighting(const std::shared_ptr<PGK_Light> &light, Vec3 &lightDir, const Vec3 &viewDir, const Vec3 &normal, const Vec3 &surfacePos);
    inline cVec3 calculateFog(const cVec3 &color, const Vec3 &surfacePos, const Vec3 &cameraPos, float fogStart = 50.0f, float fogEnd = 100.0f);
    inline void getLightTypeVariables(const std::shared_ptr<PGK_Light> &light, const Vec3 &surfacePos, Vec3 &lightDir, float &attenuation, float &spotEffect);
};

#endif // PGK_DRAW_H
