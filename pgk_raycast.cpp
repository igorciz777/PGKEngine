#include "pgk_raycast.h"
#include "pgk_gameobject.h"

RaycastHit PGK_Raycast::raycast(const Vec3 &orig, const Vec3 &dir, const float &length, const std::vector<std::shared_ptr<PGK_GameObject> > &gameObjects)
{
    RaycastHit hit;
    hit.distance = length;
    for (const auto& gameObject : gameObjects) {
        if (gameObject->getMeshes().size() > 0) {
            for (const auto& mesh : gameObject->getMeshes()) {
                for (size_t i = 0; i < mesh.indices.size(); i += 3) {
                    Vec3 v0 = mesh.vertices[mesh.indices[i]].position;
                    Vec3 v1 = mesh.vertices[mesh.indices[i + 1]].position;
                    Vec3 v2 = mesh.vertices[mesh.indices[i + 2]].position;

                    Vec3 e1 = v1 - v0;
                    Vec3 e2 = v2 - v0;
                    Vec3 p = dir.cross(e2);
                    float det = e1.dot(p);

                    if (det > -EPS && det < EPS) continue;

                    float invDet = 1.0f / det;
                    Vec3 t = orig - v0;
                    float u = t.dot(p) * invDet;
                    if (u < 0 || u > 1) continue;

                    Vec3 q = t.cross(e1);
                    float v = dir.dot(q) * invDet;
                    if (v < 0 || u + v > 1) continue;

                    float distance = e2.dot(q) * invDet;
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
    }
    return hit;
}
