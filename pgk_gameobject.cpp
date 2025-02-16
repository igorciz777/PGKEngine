#include "pgk_gameobject.h"
#include "pgk_draw.h"

#include <QThread>

PGK_GameObject::PGK_GameObject()
    : localPosition(0, 0, 0), localEuler(0, 0, 0), localRotation(0, 0, 0, 1), localScale(1, 1, 1), parent(nullptr) {
    this->isVisible = false;
    this->name = "gameObject";
}

PGK_GameObject::~PGK_GameObject() {
    for (auto& child : children) {
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
    uint32_t verticesSize = 0;
    for (const auto& mesh : meshes) {
        verticesSize += mesh.vertices.size();
    }
    this->clippedVertices.resize(verticesSize);
    this->clippedNormals.resize(verticesSize);
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
    for (auto& child : children) {
        child->update(deltaTime);
    }
}

void PGK_GameObject::render(PGK_View *view, const Mat4& viewMatrix, const Mat4 &projViewMatrix, const Vec3& cameraPos, const std::vector<PGK_Light> &lights) {
    for (auto& child : children) {
        child->render(view,viewMatrix,projViewMatrix,cameraPos,lights);
    }
    if(!isVisible) return;
    const Mat4 worldTransform = getWorldTransform();
    const Mat4 modelViewInvTrs = (viewMatrix * worldTransform.inverse()).transpose();
    const Mat4 projModelView = projViewMatrix * worldTransform;

    for(size_t i = 0; i < this->gameObjectMesh.size(); i++) {
        Mesh mesh = this->gameObjectMesh[i];
        for (size_t i = 0; i < mesh.vertices.size(); i++) {
            this->clippedVertices[i] = projModelView * Vec4(mesh.vertices[i].position);
            this->clippedNormals[i] = modelViewInvTrs * Vec4(mesh.vertices[i].normal);
        }
        for (size_t i = 0; i < mesh.indices.size(); i += 3) {
            if(clippedVertices[mesh.indices[i]].w   < view->nearClip) continue;
            if(clippedVertices[mesh.indices[i+1]].w < view->nearClip) continue;
            if(clippedVertices[mesh.indices[i+2]].w < view->nearClip) continue;
            if(clippedVertices[mesh.indices[i]].w   > view->farClip)  continue;
            if(clippedVertices[mesh.indices[i+1]].w > view->farClip)  continue;
            if(clippedVertices[mesh.indices[i+2]].w > view->farClip)  continue;

            Vec3 ndc0 = PGK_Math::clipToNDC(clippedVertices[mesh.indices[i]]);
            Vec3 ndc1 = PGK_Math::clipToNDC(clippedVertices[mesh.indices[i + 1]]);
            Vec3 ndc2 = PGK_Math::clipToNDC(clippedVertices[mesh.indices[i + 2]]);

            // if(ndc0.z < -view->nearClip || ndc0.z > view->farClip) continue;
            // if(ndc1.z < -view->nearClip || ndc1.z > view->farClip) continue;
            // if(ndc2.z < -view->nearClip || ndc2.z > view->farClip) continue;

            Vec3 screen0 = PGK_Math::projectionToScreen(ndc0, view->resWidth, view->resHeight, view->nearClip, view->farClip);
            Vec3 screen1 = PGK_Math::projectionToScreen(ndc1, view->resWidth, view->resHeight, view->nearClip, view->farClip);
            Vec3 screen2 = PGK_Math::projectionToScreen(ndc2, view->resWidth, view->resHeight, view->nearClip, view->farClip);

            if((ndc1 - ndc0).cross(ndc2 - ndc0).z < 0) continue;

            Triangle t = {
                Vec3(worldTransform.m03, worldTransform.m13, worldTransform.m23),
                clippedVertices[mesh.indices[i]],
                clippedVertices[mesh.indices[i+1]],
                clippedVertices[mesh.indices[i+2]],
                ndc0,ndc1,ndc2,
                screen0,screen1,screen2,
                clippedNormals[mesh.indices[i]],
                clippedNormals[mesh.indices[i+1]],
                clippedNormals[mesh.indices[i+2]],
                mesh.vertices[mesh.indices[i]].texCoord,
                mesh.vertices[mesh.indices[i + 1]].texCoord,
                mesh.vertices[mesh.indices[i + 2]].texCoord
            };
            PGK_Draw::drawTriangle(view->canvas, mesh.material, t, view->_zbuffer, lights, cameraPos);
        }
    }
}

void PGK_GameObject::addRigidbody(std::shared_ptr<PGK_Rigidbody> rigidbody) {
    this->rigidbody = rigidbody;
    rigidbody->attachToObject(this);
}

std::shared_ptr<PGK_Rigidbody> PGK_GameObject::getRigidbody() const {
    return rigidbody;
}
