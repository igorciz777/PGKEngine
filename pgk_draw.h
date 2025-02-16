#ifndef PGK_DRAW_H
#define PGK_DRAW_H

#include <QColor>
#include <QImage>

#include "pgk_light.h"
#include "pgk_math.h"

namespace PGK_Draw
{
    inline void drawPixel(QImage &target, const QColor &color, int16_t x0, int16_t y0);
    inline void drawLine(QImage &target, const QColor &color, int16_t x0, int16_t y0, int16_t x1, int16_t y1);
    inline void drawCircle(QImage &target, const QColor &color, int16_t x0, int16_t y0, float radius);
    void drawTriangle(QImage &target, const Material &material, const Triangle &triangle, std::vector<float> &zBuffer, const std::vector<PGK_Light> &lights, const Vec3 &cameraPos);
    void drawText(QImage &target, const QString &text, uint8_t size,int16_t x0, int16_t y0, QColor color);

    inline void floodFill(QImage &target, QColor color, QColor newColor, int16_t x0, int16_t y0);
    inline void scanLine(QImage &target, QColor color, std::vector<QPoint> polygonPoints);

    inline QColor getColor(const QImage &image, int16_t x, int16_t y);
    inline QColor getInterpolatedColor(const QImage &image, float x, float y);
    inline QColor calculatePhongLighting(const PGK_Light &light, const Vec3 &lightDir, const Vec3 &viewDir, const Vec3 &reflectDir, const Vec3 &normal);
    inline QColor calculateBlinnPhongLighting(const PGK_Light &light, const Vec3 &lightDir, const Vec3 &viewDir, const Vec3 &normal);
    inline QColor calculateFlatLighting(const PGK_Light &light, const Vec3 &lightDir, const Vec3 &normal);
};

#endif // PGK_DRAW_H
