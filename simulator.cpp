#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "engine.h"
#include "objects.h"
#include "camera.h"

#define WIN_WIDTH 1080
#define WIN_HEIGHT 720
#define WIN_TITLE "3D Engine"

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    Camera* cameraInstance = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (!cameraInstance) return;

    cameraInstance->changefov(yoffset);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    Camera* cameraInstance = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (!cameraInstance) return;

    if (cameraInstance->firstMouse) {
        cameraInstance->lastX = xpos;
        cameraInstance->lastY = ypos;
        cameraInstance->firstMouse = false;
    }

    // get the change in (x,y)
    float xOffset = xpos - cameraInstance->lastX;
    float yOffset = cameraInstance->lastY - ypos; // y is inverse
    cameraInstance->lastX = xpos;
    cameraInstance->lastY = ypos;


    cameraInstance->rotateCamera(xOffset * cameraInstance->sensitivity, yOffset * cameraInstance->sensitivity);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, NULL, NULL);
    if (!window) return -1;
    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST); // Enable depth testing

    // Create the engine and add a rigid body (the cube)
    PhysicsEngine engine;
    RigidBody cube = {glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(1.0f, 3.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,3.0f,2.0f), ShapeType::Cuboid, glm::vec3(1.0f, 0.0f, 1.0f), 1.0f};
    engine.addRigidObject(cube);

    // Create Camera (Positioned initially to view the cube)
    Camera camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 60.0f, 0.1f, 100.0f, 1.5f, 0.1f);


    // scrollwheel actions for fov
    glfwSetWindowUserPointer(window, &camera);
    glfwSetScrollCallback(window, scroll_callback);

    // mouse actions for rotation
    glfwSetCursorPosCallback(window, mouse_callback);

    // update for when screen size changes
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // The timing rates of the function
    float prevTime = 0.0f;
    float deltaTime = 0.0f;

    // Main update loop
    while (!glfwWindowShouldClose(window)) {
        //update objects
        double currTime = glfwGetTime();
        deltaTime = currTime - prevTime;
        prevTime = currTime;
        engine.update(deltaTime);

        // Handle camera movement
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.moveForward(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.moveBackward(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.moveLeft(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.moveRight(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            camera.moveUp(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            camera.moveDown(deltaTime);
        }

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            engine.addRigidObject(cube);
        }

        // Render the scene (objects will stay still, only the camera moves)
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        engine.render(camera, width, height);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}
