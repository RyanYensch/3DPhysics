#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <GLFW/glfw3.h>
#include "camera.h"

void Camera::changefov(double y) {
    if (y > 0 && fov > 1) {
        fov--;
    } else if (y < 0 && fov < 120) {
        fov++;
    }
}