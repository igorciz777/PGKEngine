#include "pgk_math.h"

cVec3 PGK_Math::interpolatecVec3(const cVec3 &p00, const cVec3 &p10, const cVec3 &p01, const cVec3 &p11, const float &a, const float &b)
{
    return cVec3(
        p00.x * (1 - a) * (1 - b) + p10.x * a * (1 - b) + p01.x * (1 - a) * b + p11.x * a * b,
        p00.y * (1 - a) * (1 - b) + p10.y * a * (1 - b) + p01.y * (1 - a) * b + p11.y * a * b,
        p00.z * (1 - a) * (1 - b) + p10.z * a * (1 - b) + p01.z * (1 - a) * b + p11.z * a * b);
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
