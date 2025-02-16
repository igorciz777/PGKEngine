#ifndef PGK_RIGIDBODY_H
#define PGK_RIGIDBODY_H

#include "pgk_math.h"
#include <memory>

class PGK_GameObject;

class PGK_Rigidbody {
public:
    PGK_Rigidbody();
    ~PGK_Rigidbody();

    void setMass(float mass);
    void setVelocity(const Vec3& velocity);
    void setGravity(const Vec3& gravity);

    void addForce(const Vec3& force);
    void update(float deltaTime);

    Vec3 getVelocity() const { return velocity; }
    Vec3 getPosition() const { return position; }

    void attachToObject(PGK_GameObject *object);

private:
    PGK_GameObject *attachedObject;

    Vec3 position = Vec3(0, 0, 0);
    Vec3 velocity = Vec3(0, 0, 0);
    Vec3 acceleration = Vec3(0, 0, 0);
    Vec3 gravity = Vec3(0, -9.81f, 0); // Default gravity
    float mass = 1.0f;

    void applyGravity(float deltaTime);
};

#endif // PGK_RIGIDBODY_H
