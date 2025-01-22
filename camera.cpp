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

    // So that you dont do a backflip
    if (pitch > 89.0f) pitch = 89.0f;
    else if (pitch < -89.0f) pitch = -89.0f;

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    // calculate the new front
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldup));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::moveForward(float deltaTime) {
    position += deltaTime * speed * front;
}

void Camera::moveBackward(float deltaTime) {
    position -= deltaTime * speed * front;
}

void Camera::moveLeft(float deltaTime) {
    position -= glm::normalize(glm::cross(front, up)) * speed * deltaTime;
}

void Camera::moveRight(float deltaTime) {
    position += glm::normalize(glm::cross(front, up)) * speed * deltaTime;
}

void Camera::moveUp(float deltaTime) {
    position.y += deltaTime * speed;
}

void Camera::moveDown(float deltaTime) {
    position.y -= deltaTime * speed;
}