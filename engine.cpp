#include "engine.h"

void PhysicsEngine::addObject(const RigidBody& obj) {
    objects.push_back(obj);
}

void PhysicsEngine::update(float deltaTime) {
    for (auto& obj : objects) {
        obj.velocity += obj.acceleration * deltaTime;
        obj.position += obj.velocity * deltaTime;
    }
}
