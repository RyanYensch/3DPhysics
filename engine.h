#ifndef ENGINE_H
#define ENGINE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <GLFW/glfw3.h>
#include "objects.h"

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float fov;
    float nearPlane;
    float farPlane;
    float speed;

    Camera(glm::vec3 pos, glm::vec3 tgt, glm::vec3 upDir, float fieldOfView, float nearP, float farP, float spd)
        : position(pos), front(tgt), up(upDir), fov(fieldOfView), nearPlane(nearP), farPlane(farP), speed(spd) {}

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }
};

class PhysicsEngine {
public:
    void addObject(const RigidBody& obj);
    void update(float deltaTime);
    void render(const Camera& camera, int windowWidth, int windowHeight);

private:
    std::vector<RigidBody> objects;
};

#endif // ENGINE_H
