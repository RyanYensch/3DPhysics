#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <GLFW/glfw3.h>

#define WIN_WIDTH 1080
#define WIN_HEIGHT 720

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldup;
    float fov;
    float nearPlane;
    float farPlane;
    float speed;
    float yaw;
    float pitch;
    float sensitivity;
    float lastX = WIN_WIDTH / 2.0f;
    float lastY = WIN_HEIGHT / 2.0f;
    bool firstMouse = true;

    Camera(glm::vec3 pos, glm::vec3 tgt, glm::vec3 upDir, float fieldOfView, float nearP, float farP, float spd, float sens)
        : position(pos), front(tgt), up(upDir), worldup(upDir), fov(fieldOfView), nearPlane(nearP), farPlane(farP), speed(spd), yaw(-90.0f), pitch(0.0f), sensitivity(sens) {
            updateCameraVectors();
        }

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    void changefov(double y);
    void rotateCamera(float xoffset, float yoffset);
    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void moveUp(float deltaTime);
    void moveDown(float deltaTime);

private:
    void updateCameraVectors();
};


#endif // CAMERA_H