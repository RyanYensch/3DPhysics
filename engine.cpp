#include <glm/glm.hpp>
#include <vector>
#include "engine.h"

struct RigidBody {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float mass;
};

class PhysicsEngine {
public:
    void addObject(const RigidBody& obj) {
        objects.push_back(obj);
    }

    void update(float deltaTime) {
        for (auto& obj: objects) {
            obj.velocity += obj.acceleration * deltaTime;
            obj.position += obj.velocity * deltaTime;
        }
    }

private:
    std::vector<RigidBody> objects;
};