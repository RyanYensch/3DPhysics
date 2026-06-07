#ifndef OBJECTS_H
#define OBJECTS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <GLFW/glfw3.h>
#include <utility>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>



enum class ShapeType {
    Cuboid,
    Sphere
};

class ShapeBase {
public:
    std::vector<glm::vec3> verticies;
    std::vector<glm::vec3> normals;

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
    float mass;

    glm::quat orientation;
    glm::vec3 angularVelocity;
    glm::vec3 torque;
    glm::vec3 localInertia;

    glm::vec3 scale;
    ShapeType shape;
    glm::vec<3, GLfloat> colour;

    RigidBody(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, glm::vec3 size, ShapeType shp, glm::vec<3, GLfloat> col, float m)
        : position(pos), velocity(vel), acceleration(acc), mass(m), 
          orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
          angularVelocity(glm::vec3(0.0f)), torque(glm::vec3(0.0f)), 
          localInertia(glm::vec3(0.0f)), scale(size), shape(shp), colour(col) {

            initShape(shape, scale);

            if (mass > 0.0f) {
                localInertia.x = (1.0f / 12.0f) * mass * (scale.y * scale.y + scale.z * scale.z);
                localInertia.y = (1.0f / 12.0f) * mass * (scale.x * scale.x + scale.z * scale.z);
                localInertia.z = (1.0f / 12.0f) * mass * (scale.x * scale.x + scale.y * scale.y);
            }
        }
};

class SimpleBody : public ShapeBase {
public:
    glm::vec3 position;
    glm::vec3 scale;
    ShapeType shape;
    glm::vec<3, GLfloat> colour;

    glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); 

    SimpleBody(glm::vec3 pos, glm::vec3 size, ShapeType shp, glm::vec<3, GLfloat> col)
        : position(pos), scale(size), shape(shp), colour(col) {
            initShape(shape, scale);
        }
};


#endif // OBJECTS_H