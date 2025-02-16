#include "pgk_rigidbody.h"
#include "pgk_gameobject.h"

PGK_Rigidbody::PGK_Rigidbody() {}
PGK_Rigidbody::~PGK_Rigidbody() {}

void PGK_Rigidbody::setMass(float mass) {
    this->mass = mass;
}

void PGK_Rigidbody::setVelocity(const Vec3& velocity) {
    this->velocity = velocity;
}

void PGK_Rigidbody::setGravity(const Vec3& gravity) {
    this->gravity = gravity;
}

void PGK_Rigidbody::addForce(const Vec3& force) {
    acceleration += force / mass;
}

void PGK_Rigidbody::update(float deltaTime) {
    applyGravity(deltaTime);

    // Update velocity and position
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;

    // Reset acceleration
    acceleration = Vec3(0, 0, 0);

    // Update attached object's position
    attachedObject->setLocalPosition(position);
}

void PGK_Rigidbody::applyGravity(float deltaTime) {
    velocity += gravity * deltaTime;
}

void PGK_Rigidbody::attachToObject(PGK_GameObject *object) {
    attachedObject = object;
    position = object->getWorldPosition();
}
