#ifndef PGK_CAMERA_H
#define PGK_CAMERA_H

#include "pgk_math.h"
#include "pgk_gameobject.h"

class PGK_Camera : public PGK_GameObject {
public:
    enum class Mode {
        Free,
        Attached
    };

    PGK_Camera();
    PGK_Camera(float fov, float aspect);
    ~PGK_Camera();

    void setMode(Mode mode);
    void setFov(float fov);
    void setAspectRatio(float aspect);

    void updateCamera(float deltaTime);

    Mat4 getViewMatrix() const;
    Mat4 getProjectionMatrix(const float &nearClip, const float &farClip) const;

private:
    Mode mode = Mode::Free;

    // Camera properties
    Mat4 viewMatrix = Mat4::IDENTITY;
    float fov = 45.0f;
    float aspectRatio = 1.0f;
    float yaw = 0.0f;
    float pitch = 0.0f;

    // Attached mode
    Vec3 attachmentOffset = Vec3(0, 0, 0);

    // Free mode
    float moveSpeed = 50.0f;
    float lookSpeed = 1.0f;

    void updateFreeFly(float deltaTime);
    void updateAttached(float deltaTime);
};

#endif // PGK_CAMERA_H
