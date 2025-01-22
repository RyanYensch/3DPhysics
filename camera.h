#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <GLFW/glfw3.h>
#include "camera.h"

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float fov;
    float nearPlane;
    float farPlane;
    float speed;

    Camera(glm::vec3 pos, glm::vec3 tgt, glm::vec3 upDir, float fieldOfView, float nearP, float farP, float spd)
        : position(pos), front(tgt), up(upDir), fov(fieldOfView), nearPlane(nearP), farPlane(farP), speed(spd) {}

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    void changefov(double y);
};


#endif // CAMERA_H