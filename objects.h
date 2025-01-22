#ifndef OBJECTS_H
#define OBJECTS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <GLFW/glfw3.h>

class RigidBody {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float mass;
    float scale;


    RigidBody(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, float m, float size)
        : position(pos), velocity(vel), acceleration(acc), mass(m), scale(size) {}
};


#endif // OBJECTS_H