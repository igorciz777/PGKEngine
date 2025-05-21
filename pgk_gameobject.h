#ifndef PGK_GAMEOBJECT_H
#define PGK_GAMEOBJECT_H

#include "pgk_math.h"
#include "pgk_view.h"
#include "pgk_rigidbody.h"
#include "pgk_obj.h"
#include <vector>
#include <memory>

class PGK_Light;

class PGK_GameObject {
public:
    PGK_GameObject();
    ~PGK_GameObject();

    std::function<void(PGK_GameObject*, float)> onUpdate;

    void setParent(PGK_GameObject* parent);
    void addChild(std::shared_ptr<PGK_GameObject> child);

    void setLocalPosition(const Vec3& position);
    void setLocalRotation(const Quat& rotation);
    void setLocalEuler(const Vec3& eulerAngles, const Quat::RotationOrder& order);
    void setLocalScale(const Vec3& scale);
    void setMeshes(const std::vector<Mesh> &meshes);
    void setName(const QString &name);

    Vec3 getLocalPosition() const;
    Vec3 getLocalEuler() const;
    Quat getLocalRotation() const;
    Vec3 getWorldPosition() const;
    Quat getWorldRotation() const;
    Vec3 getWorldEuler() const;
    Vec3 getWorldScale() const;
    QString getName() const;
    std::vector<std::shared_ptr<PGK_GameObject>> getChildren() const;
    PGK_GameObject* getParent();
    std::vector<Mesh> getMeshes() const;
    
    Mat4 getLocalTransform() const;
    Mat4 getWorldTransform() const;

    void update(float &deltaTime);
    void getTriangleBuffer(std::vector<Triangle> &triangleBuffer, PGK_View *view, const Mat4& viewMatrix, const Mat4& projectionMatrix);
    uint64_t calcTriangleBufferSize();

    void addRigidbody(std::shared_ptr<PGK_Rigidbody> rigidbody);
    std::shared_ptr<PGK_Rigidbody> getRigidbody() const;

    bool receiveShadows=false;
    bool castShadows=false;
    bool isVisible=true;

private:
    QString name;

    Vec3 localPosition;
    Vec3 localEuler;
    Quat localRotation;
    Vec3 localScale;

    std::vector<Mesh> gameObjectMesh;

    PGK_GameObject* parent;
    std::vector<std::shared_ptr<PGK_GameObject>> children;
    std::shared_ptr<PGK_Rigidbody> rigidbody;
};

#endif // PGK_GAMEOBJECT_H
