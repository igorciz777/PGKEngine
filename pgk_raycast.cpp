#include "pgk_raycast.h"


bool PGK_Raycast::intersectTriangle(
    const Vec3& origin, const Vec3& direction,
    const Vec3& v0, const Vec3& v1, const Vec3& v2,
    float& t, Vec3& normal
    ) {
    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;
    Vec3 h = direction.cross(edge2);
    float a = edge1.dot(h);

    if (std::abs(a) < EPS) return false;

    float f = 1.0f / a;
    Vec3 s = origin - v0;
    float u = f * s.dot(h);
    if (u < 0 || u > 1) return false;

    Vec3 q = s.cross(edge1);
    float v = f * direction.dot(q);
    if (v < 0 || u + v > 1) return false;

    t = f * edge2.dot(q);
    if (t < EPS) return false;

    normal = edge1.cross(edge2).normalize();
    return true;
}
