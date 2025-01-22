#ifndef OBJECTS_H
#define OBJECTS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <GLFW/glfw3.h>

enum class ShapeType { 
    Cuboid,
    Sphere
};

class RigidBody {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 scale;
    ShapeType shape;
    std::vector<glm::vec3> verticies;
    glm::vec<3, GLfloat> colour;
    float mass;


    RigidBody(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, glm::vec3 size, ShapeType shp, glm::vec<3, GLfloat> col, float m)
        : position(pos), velocity(vel), acceleration(acc), mass(m), scale(size), shape(shp), colour(col) {
            initShape();
        }


private:
    void initShape();
    void initCuboid();
};


#endif // OBJECTS_H