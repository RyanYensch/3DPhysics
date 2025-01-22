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
    float mass;


    RigidBody(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, glm::vec3 size, ShapeType shp, float m)
        : position(pos), velocity(vel), acceleration(acc), mass(m), scale(size), shape(shp) {
            initShape();
        }


private:
    std::vector<glm::vec3> verticies;

    void initShape() {
        switch(shape) {
            case ShapeType::Cuboid:
                initCuboid();
                break;
            default:
                break;
        }
    }

    void initCuboid() {
        
    }
};


#endif // OBJECTS_H