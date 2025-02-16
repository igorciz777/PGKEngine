#include "pgk_math.h"

QColor PGK_Math::interpolateColor(const QColor &p00, const QColor &p10, const QColor &p01, const QColor &p11, const float &a, const float &b)
{
    return QColor(
        p00.red() * (1 - a) * (1 - b) + p10.red() * a * (1 - b) + p01.red() * (1 - a) * b + p11.red() * a * b,
        p00.green() * (1 - a) * (1 - b) + p10.green() * a * (1 - b) + p01.green() * (1 - a) * b + p11.green() * a * b,
        p00.blue() * (1 - a) * (1 - b) + p10.blue() * a * (1 - b) + p01.blue() * (1 - a) * b + p11.blue() * a * b,
        p00.alpha() * (1 - a) * (1 - b) + p10.alpha() * a * (1 - b) + p01.alpha() * (1 - a) * b + p11.alpha() * a * b);
}

Vec3 PGK_Math::getBarycentric(const Vec2 &A, const Vec2 &B, const Vec2 &C, const Vec2 &P)
{
    Mat2 v0(P.x - A.x, C.x - A.x, P.y - A.y, C.y - A.y);
    Mat2 v1(B.x - A.x, C.x - A.x, B.y - A.y, C.y - A.y);
    Mat2 w0(B.x - A.x, P.x - A.x, B.y - A.y, P.y - A.y);
    Mat2 w1(B.x - A.x, C.x - A.x, B.y - A.y, C.y - A.y);

    float v = v0.determinant() / v1.determinant();
    float w = w0.determinant() / w1.determinant();
    float u = 1 - v - w;

    return Vec3(u,v,w);
}

Vec3 PGK_Math::getBarycentric(const Vec3 &A, const Vec3 &B, const Vec3 &C, const Vec3 &P)
{
    Mat2 v0(P.x - A.x, C.x - A.x, P.y - A.y, C.y - A.y);
    Mat2 v1(B.x - A.x, C.x - A.x, B.y - A.y, C.y - A.y);
    Mat2 w0(B.x - A.x, P.x - A.x, B.y - A.y, P.y - A.y);
    Mat2 w1(B.x - A.x, C.x - A.x, B.y - A.y, C.y - A.y);

    float v = v0.determinant() / v1.determinant();
    float w = w0.determinant() / w1.determinant();
    float u = 1 - v - w;

    return Vec3(u,v,w);
}

uint64_t PGK_Math::fastpow(uint64_t base, uint64_t power)
{
    uint64_t result = 1;
    while(power > 0) {

        if(power & 1) {
            result = (result*base) % MOD;
        }
        base = (base * base) % MOD;
        power = power / 2;
    }
    return result;
}
