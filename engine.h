#ifndef ENGINE_H
#define ENGINE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

struct RigidBody {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float mass;
};

class Camera {
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float fov;

    Camera()
        : position(0.0f, 0.0f, 3.0f),
          front(0.0f, 0.0f, -1.0f),
          up(0.0f, 1.0f, 0.0f),
          fov(45.0f) {}

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }
};

class PhysicsEngine {
public:
    void addObject(const RigidBody& obj);
    void update(float deltaTime);

private:
    std::vector<RigidBody> objects;
};


#endif // ENGINE_H