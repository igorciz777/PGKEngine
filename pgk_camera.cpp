#include "pgk_camera.h"
#include "pgk_input.h"

PGK_Camera::PGK_Camera() : PGK_GameObject() {}
PGK_Camera::PGK_Camera(float fov, float aspect) : PGK_GameObject() {
    this->fov = fov;
    this->aspectRatio = aspect;
}
PGK_Camera::~PGK_Camera() {}

void PGK_Camera::setMode(Mode mode) {
    this->mode = mode;
}

void PGK_Camera::setFov(float fov) {
    this->fov = fov;
}

void PGK_Camera::setAspectRatio(float aspect) {
    this->aspectRatio = aspect;
}

void PGK_Camera::updateCamera(float deltaTime) {
    switch (mode) {
    case Mode::Free:
        updateFreeFly(deltaTime);
        break;
    case Mode::Attached:
        updateAttached(deltaTime);
        break;
    }
}

Mat4 PGK_Camera::getViewMatrix() const {
    return viewMatrix;
}

Mat4 PGK_Camera::getProjectionMatrix(const float &nearClip, const float &farClip) const {
    return Mat4::PerspectiveProjection(fov, aspectRatio, nearClip, farClip);
}

void PGK_Camera::updateFreeFly(float deltaTime) {
    auto& input = PGK_Input::instance();

    yaw = 0;
    pitch = 0;

    if (input.getKey(Qt::Key_Up)) pitch += lookSpeed * deltaTime;
    if (input.getKey(Qt::Key_Down)) pitch -= lookSpeed * deltaTime;
    if (input.getKey(Qt::Key_Left)) yaw += lookSpeed * deltaTime;
    if (input.getKey(Qt::Key_Right)) yaw -= lookSpeed * deltaTime;

    //reset view
    if (input.getKey(Qt::Key_R)) setLocalRotation(Quat::IDENTITY);

    Quat yawQ = Quat(Vec3(0, 1, 0), yaw);
    Quat pitchQ = Quat(Vec3(1, 0, 0), pitch);

    setLocalRotation((yawQ * getLocalRotation() * pitchQ).normalize());


    Vec3 forward = getLocalRotation() * Vec3(0, 0, -1);
    Vec3 right = getLocalRotation() * Vec3(1, 0, 0);
    Vec3 up = getLocalRotation() * Vec3(0, 1, 0);


    if (input.getKey(Qt::Key_W)) setLocalPosition(getLocalPosition() + forward * moveSpeed * deltaTime);
    if (input.getKey(Qt::Key_S)) setLocalPosition(getLocalPosition() - forward * moveSpeed * deltaTime);
    if (input.getKey(Qt::Key_A)) setLocalPosition(getLocalPosition() - right * moveSpeed * deltaTime);
    if (input.getKey(Qt::Key_D)) setLocalPosition(getLocalPosition() + right * moveSpeed * deltaTime);
    if (input.getKey(Qt::Key_Q)) setLocalPosition(getLocalPosition() - up * moveSpeed * deltaTime);
    if (input.getKey(Qt::Key_E)) setLocalPosition(getLocalPosition() + up * moveSpeed * deltaTime);

    viewMatrix = PGK_Math::lookAtMatrix(getLocalPosition(), getLocalPosition() + forward, up);
}

void PGK_Camera::updateAttached(float) {
    if (auto object = getParent()) {
        viewMatrix = PGK_Math::lookAtMatrix(getWorldPosition(), object->getWorldPosition(), Vec3(0, 1, 0));
    }
}
