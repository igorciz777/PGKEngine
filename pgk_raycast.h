#ifndef PGK_RAYCAST_H
#define PGK_RAYCAST_H

#include "pgk_math.h"
#include <vector>
#include <memory>

class PGK_GameObject;

struct RaycastHit {
    Vec3 point;
    Vec3 normal;
    float distance;
    std::shared_ptr<PGK_GameObject> object;
};

class PGK_Raycast {
public:
    static bool raycast(const Vec3& origin, const Vec3& direction, float maxDistance, RaycastHit& hit, const std::vector<std::shared_ptr<PGK_GameObject>>& objects);

private:
    static bool intersectTriangle(
        const Vec3& origin, const Vec3& direction,
        const Vec3& v0, const Vec3& v1, const Vec3& v2,
        float& t, Vec3& normal
        );
};

#endif // PGK_RAYCAST_H
