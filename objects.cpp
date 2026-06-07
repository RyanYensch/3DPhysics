#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <GLFW/glfw3.h>
#include "objects.h"

void ShapeBase::initShape(ShapeType shape, glm::vec3& scale) {
    switch(shape) {
        case ShapeType::Cuboid:
            ShapeBase::initCuboid(scale);
            break;
        default:
            break;
    }
}

void ShapeBase::initCuboid(glm::vec3& scale) {
    for (int axis = 0; axis < 3; axis++) {
        for (const int sign : {-1, 1}) {
            glm::vec3 normal(0.0f);
            normal[axis] = (float)sign;

            for (int i = 0; i < 4; i++) {
                glm::vec3 vertex(0.0f);

                // face direction sign
                vertex[axis] = sign * 0.5f;

                // other 2 verticies
                vertex[(axis + 1) % 3] = ((i == 0 || i == 3) ? -1.0f : 1.0f) * 0.5f;
                vertex[(axis + 2) % 3] = ((i == 0 || i == 1) ? -1.0f : 1.0f) * 0.5f;

                verticies.push_back(vertex);
                normals.push_back(normal);
            }
        }
    }
}