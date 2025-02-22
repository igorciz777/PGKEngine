#include "pgk_gameobject.h"

#include <QThread>

PGK_GameObject::PGK_GameObject()
    : localPosition(0, 0, 0), localEuler(0, 0, 0), localRotation(0, 0, 0, 1), localScale(1, 1, 1), parent(nullptr) {
    this->isVisible = false;
    this->name = "gameObject";
}

PGK_GameObject::~PGK_GameObject() {
    for (const auto& child : children) {
        child->setParent(nullptr);
    }
}

void PGK_GameObject::setParent(PGK_GameObject* parent) {
    this->parent = parent;
}

void PGK_GameObject::addChild(std::shared_ptr<PGK_GameObject> child) {
    child->setParent(this);
    children.push_back(child);
}

void PGK_GameObject::setLocalPosition(const Vec3& position) {
    localPosition = position;
}

void PGK_GameObject::setLocalRotation(const Quat& rotation) {
    localRotation = rotation;
    localEuler = rotation.toEuler(Quat::RotationOrder::XYZ);
}

void PGK_GameObject::setLocalEuler(const Vec3 &eulerAngles, const Quat::RotationOrder &order)
{
    localEuler = eulerAngles;
    localRotation = Quat(eulerAngles, order);
}

void PGK_GameObject::setLocalScale(const Vec3& scale) {
    localScale = scale;
}

void PGK_GameObject::setMeshes(const std::vector<Mesh> &meshes)
{
    this->gameObjectMesh = meshes;
    this->isVisible = true;
}

void PGK_GameObject::setName(const QString &name)
{
    this->name = name;
}

Vec3 PGK_GameObject::getLocalPosition() const
{
    return this->localPosition;
}

Vec3 PGK_GameObject::getLocalEuler() const
{
    return this->localEuler;
}

Quat PGK_GameObject::getLocalRotation() const
{
    return this->localRotation;
}

Vec3 PGK_GameObject::getWorldPosition() const {
    Mat4 worldTransform = getWorldTransform();
    return Vec3(worldTransform.m03, worldTransform.m13, worldTransform.m23);
}

Quat PGK_GameObject::getWorldRotation() const {
    Mat4 worldTransform = getWorldTransform();
    return Quat(worldTransform);
}

Vec3 PGK_GameObject::getWorldEuler() const
{
    return getWorldRotation().toEuler(Quat::RotationOrder::XYZ);
}

Vec3 PGK_GameObject::getWorldScale() const {
    Mat4 worldTransform = getWorldTransform();
    return Vec3(worldTransform.m00, worldTransform.m11, worldTransform.m22);
}

QString PGK_GameObject::getName() const
{
    return this->name;
}

std::vector<std::shared_ptr<PGK_GameObject> > PGK_GameObject::getChildren() const
{
    return this->children;
}

PGK_GameObject *PGK_GameObject::getParent()
{
    return this->parent;
}

std::vector<Mesh> PGK_GameObject::getMeshes() const
{
    return this->gameObjectMesh;
}

Mat4 PGK_GameObject::getLocalTransform() const {
    return Mat4::Transform(localPosition, localRotation, localScale);
}

Mat4 PGK_GameObject::getWorldTransform() const {
    if (parent) {
        return parent->getWorldTransform() * getLocalTransform();
    }
    return getLocalTransform();
}

void PGK_GameObject::update(float &deltaTime) {
    // Call custom update
    if(onUpdate) {
        onUpdate(this, deltaTime);
    }

    // Update children
    for (const auto& child : children) {
        child->update(deltaTime);
    }
}

void PGK_GameObject::getTriangleBuffer(std::vector<Triangle> &triangleBuffer, PGK_View *view, const Mat4 &viewMatrix, const Mat4 &projectionMatrix)
{
    for (const auto& child : children) {
        child->getTriangleBuffer(triangleBuffer,view,viewMatrix,projectionMatrix);
    }
    if(!isVisible) return;
    const Mat4 worldTransform = getWorldTransform();
    const Mat4 modelViewInvTrs = PGK_Math::normalMatrix(worldTransform);

    for(size_t i = 0; i < this->gameObjectMesh.size(); i++) {
        const Mesh *mesh = &this->gameObjectMesh[i];
        std::shared_ptr<Material> materialPtr = std::make_shared<Material>(mesh->material);

        for (size_t i = 0; i < mesh->indices.size(); i += 3) {
            const Vec4 v0 = worldTransform * Vec4(mesh->vertices[mesh->indices[i]].position);
            const Vec4 v1 = worldTransform * Vec4(mesh->vertices[mesh->indices[i+1]].position);
            const Vec4 v2 = worldTransform * Vec4(mesh->vertices[mesh->indices[i+2]].position);

            const Vec4 clipped0 = projectionMatrix * viewMatrix * v0;
            const Vec4 clipped1 = projectionMatrix * viewMatrix * v1;
            const Vec4 clipped2 = projectionMatrix * viewMatrix * v2;

            if(clipped0.w < view->nearClip) continue;
            if(clipped1.w < view->nearClip) continue;
            if(clipped2.w < view->nearClip) continue;
            if(clipped0.w > view->farClip)  continue;
            if(clipped1.w > view->farClip)  continue;
            if(clipped2.w > view->farClip)  continue;

            const Vec3 ndc0 = PGK_Math::clipToNDC(clipped0);
            const Vec3 ndc1 = PGK_Math::clipToNDC(clipped1);
            const Vec3 ndc2 = PGK_Math::clipToNDC(clipped2);

            const Vec4 n0 = (modelViewInvTrs * Vec4(mesh->vertices[mesh->indices[i]].normal)).normalize();
            const Vec4 n1 = (modelViewInvTrs * Vec4(mesh->vertices[mesh->indices[i+1]].normal)).normalize();
            const Vec4 n2 = (modelViewInvTrs * Vec4(mesh->vertices[mesh->indices[i+2]].normal)).normalize();

            const Vec3 screen0 = PGK_Math::projectionToScreen(ndc0, view->resWidth, view->resHeight, view->nearClip, view->farClip);
            const Vec3 screen1 = PGK_Math::projectionToScreen(ndc1, view->resWidth, view->resHeight, view->nearClip, view->farClip);
            const Vec3 screen2 = PGK_Math::projectionToScreen(ndc2, view->resWidth, view->resHeight, view->nearClip, view->farClip);

            if((ndc1 - ndc0).cross(ndc2 - ndc0).z < 0) continue;

            const Triangle t = {
                this->receiveShadows,
                this->castShadows,
                (v0+v1+v2)/3.0f,
                v0,v1,v2,
                ndc0,ndc1,ndc2,
                screen0,screen1,screen2,
                n0,n1,n2,
                mesh->vertices[mesh->indices[i]].texCoord,
                mesh->vertices[mesh->indices[i + 1]].texCoord,
                mesh->vertices[mesh->indices[i + 2]].texCoord,
                materialPtr
            };
            triangleBuffer.push_back(t);
        }
    }
}

uint64_t PGK_GameObject::calcTriangleBufferSize()
{
    uint64_t count = 0;
    for (const auto& child : children) {
        count += child->calcTriangleBufferSize();
    }
    for(const auto& mesh : this->getMeshes()){
        count += mesh.indices.size();
    }
    return count;
}

void PGK_GameObject::addRigidbody(std::shared_ptr<PGK_Rigidbody> rigidbody) {
    this->rigidbody = rigidbody;
    rigidbody->attachToObject(this);
}

std::shared_ptr<PGK_Rigidbody> PGK_GameObject::getRigidbody() const {
    return rigidbody;
}
