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
    glm::vec3 right;
    glm::vec3 worldup;
    float fov;
    float nearPlane;
    float farPlane;
    float speed;
    float yaw;
    float pitch;

    Camera(glm::vec3 pos, glm::vec3 tgt, glm::vec3 upDir, float fieldOfView, float nearP, float farP, float spd)
        : position(pos), front(tgt), up(upDir), worldup(upDir), fov(fieldOfView), nearPlane(nearP), farPlane(farP), speed(spd), yaw(-90.0f), pitch(0.0f) {
            updateCameraVectors();
        }

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    void changefov(double y);
    void rotateCamera(float xoffset, float yoffset);

private:
    void updateCameraVectors();
};


#endif // CAMERA_H