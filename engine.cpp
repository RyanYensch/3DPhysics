#include <glm/glm.hpp>
#include <vector>

struct RigidBody {
    glm::vec3 position;
    glm::vec3 velocity;
    float mass;
};