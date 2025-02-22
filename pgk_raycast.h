#ifndef PGK_RAYCAST_H
#define PGK_RAYCAST_H

#include "pgk_math.h"

#include <memory>
#include <vector>
class PGK_GameObject;

struct RaycastHit {
    bool hit=false;
    Vec3 point;
    Vec3 normal;
    Vec3 barycentric;
    float distance;
};

class PGK_Raycast {
public:
    static RaycastHit raycast(const Vec3 &orig, const Vec3 &dir, const float &length, const std::vector<std::shared_ptr<PGK_GameObject>> &gameObjects);
private:
    
};

#endif // PGK_RAYCAST_H
