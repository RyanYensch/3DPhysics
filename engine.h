#ifndef ENGINE_H
#define ENGINE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <GLFW/glfw3.h>
#include "objects.h"
#include "camera.h"


class PhysicsEngine {
public:
    PhysicsEngine (float grav = 9.81) : gravity(grav) {}

    void addObject(const RigidBody& obj);
    void update(float deltaTime);
    void render(const Camera& camera, int windowWidth, int windowHeight);

private:
    std::vector<RigidBody> objects;
    float gravity;
};

#endif // ENGINE_H
