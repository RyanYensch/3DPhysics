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

    void addRigidObject(const RigidBody& obj);
    void addSimpleObject(const SimpleBody& obj);
    void update(float deltaTime);
    void render(const Camera& camera, int windowWidth, int windowHeight);

private:
    std::vector<RigidBody> rigidObjects;
    std::vector<SimpleBody> simpleObjects;
    float gravity;

    template <typename T>
    void renderObjects(const std::vector<T>& objects, glm::mat4 projectionView);
};

#endif // ENGINE_H
