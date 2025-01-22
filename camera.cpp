#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <GLFW/glfw3.h>
#include "camera.h"

void Camera::changefov(double y) {
    fov -= y;
    if (fov < 1) fov = 1;
    else if (fov > 120) fov = 120;
}


void Camera::rotateCamera(float xoffset, float yoffset) {
    yaw += xoffset;
    pitch += yoffset;

    // So that you dont look do a backflip
    if (pitch > 89.0f) pitch = 89.0f;
    else if (pitch < -89.0f) pitch = -89.0f;

    updateCameraVectors();
}