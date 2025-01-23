#ifndef OBJECTS_H
#define OBJECTS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <GLFW/glfw3.h>
#include <utility>

enum class ShapeType { 
    Cuboid,
    Sphere
};

class ShapeBase {
public:
    std::vector<glm::vec3> verticies;

protected:
    void initShape(ShapeType shape, glm::vec3& scale);

private:
    void initCuboid(glm::vec3& scale);
};


class RigidBody : public ShapeBase {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 scale;
    ShapeType shape;
    glm::vec<3, GLfloat> colour;
    float mass;


    RigidBody(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, glm::vec3 size, ShapeType shp, glm::vec<3, GLfloat> col, float m)
        : position(pos), velocity(vel), acceleration(acc), mass(m), scale(size), shape(shp), colour(col) {
            initShape(shape, scale);
        }
};



class SimpleBody : public ShapeBase {
public:
    glm::vec3 position;
    glm::vec3 scale;
    ShapeType shape;
    glm::vec<3, GLfloat> colour;

    SimpleBody(glm::vec3 pos, glm::vec3 size, ShapeType shp, glm::vec<3, GLfloat> col)
        : position(pos), scale(size), shape(shp), colour(col) {
            initShape(shape, scale);
        }
};


#endif // OBJECTS_H