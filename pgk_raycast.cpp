#include "pgk_raycast.h"
#include "pgk_gameobject.h"

RaycastHit PGK_Raycast::raycast(const Vec3 &orig, const Vec3 &dir, const float &length, const std::vector<std::shared_ptr<PGK_GameObject> > &gameObjects)
{
    RaycastHit hit;
    hit.distance = length;
    for (const auto& gameObject : gameObjects) {
        for (const auto& mesh : gameObject->getMeshes()) {
            for (size_t i = 0; i < mesh.indices.size(); i += 3) {
                const Vec3 v0 = mesh.vertices[mesh.indices[i]].position;
                const Vec3 v1 = mesh.vertices[mesh.indices[i + 1]].position;
                const Vec3 v2 = mesh.vertices[mesh.indices[i + 2]].position;

                const Vec3 e1 = v1 - v0;
                const Vec3 e2 = v2 - v0;
                const Vec3 p = dir.cross(e2);
                const float det = e1.dot(p);

                if (det > -EPS && det < EPS) continue;

                const float invDet = 1.0f / det;
                const Vec3 t = orig - v0;
                const float u = t.dot(p) * invDet;
                if (u < 0 || u > 1) continue;

                const Vec3 q = t.cross(e1);
                const float v = dir.dot(q) * invDet;
                if (v < 0 || u + v > 1) continue;

                const float distance = e2.dot(q) * invDet;
                if (distance < hit.distance && distance < length) {
                    hit.hit = true;
                    hit.distance = distance;
                    hit.point = orig + dir * distance;
                    hit.normal = e1.cross(e2).normalize();
                    hit.barycentric = Vec3(1 - u - v, u, v);
                }
            }
        }
    }
    return hit;
}
