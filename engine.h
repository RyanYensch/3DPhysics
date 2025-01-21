#ifndef ENGINE_H
#define ENGINE_H

#include <glm/glm.hpp>
#include <vector>

struct RigidBody {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float mass;
};

class PhysicsEngine {
public:
    void addObject(const RigidBody& obj);
    void update(float deltaTime);

private:
    std::vector<RigidBody> objects;
};


#endif // ENGINE_H