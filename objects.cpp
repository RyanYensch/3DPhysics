#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <GLFW/glfw3.h>
#include "objects.h"

void RigidBody::initShape() {
    switch(shape) {
        case ShapeType::Cuboid:
            initCuboid();
            break;
        default:
            break;
    }
}

void RigidBody::initCuboid() {
    for (int axis = 0; axis < 3; axis++) {
        for (const int sign : {-1, 1}) {
            for (int i = 0; i < 4; i++) {
                glm::vec3 vertex(0.0f);

                // face direction sign
                vertex[axis] = sign * scale[axis] * 0.5f;

                // other 2 verticies
                vertex[(axis + 1) % 3] = ((i == 0 || i == 3) ? -1.0f : 1.0f) * scale[(axis + 1) % 3] * 0.5f;
                vertex[(axis + 2) % 3] = ((i == 0 || i == 1) ? -1.0f : 1.0f) * scale[(axis + 2) % 3] * 0.5f;

                verticies.push_back(vertex);
            }
        }
    }
}