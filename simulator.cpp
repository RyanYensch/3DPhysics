#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "engine.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define WIN_TITLE "3D Engine"
#define FPS 60
#define TICK_RATE 0.016f // 60 FPS

int main() {
    // Initialize GLFW
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, NULL, NULL);
    if (!window) return -1;
    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST); // Enable depth testing

    // Create the engine and add a rigid body (the cube)
    PhysicsEngine engine;
    RigidBody cube = {glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -9.81f, 0.0f), 1.0f};
    engine.addObject(cube);

    // Create Camera (Positioned in a way to view the cube)
    Camera camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f, 0.1f, 100.0f);

    // Main update loop
    while (!glfwWindowShouldClose(window)) {
        // Update physics engine (cube's motion)
        engine.update(TICK_RATE);

        // Handle camera movement
        float moveSpeed = 0.1f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.position += moveSpeed * camera.front;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.position -= moveSpeed * camera.front;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * moveSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * moveSpeed;
        }

        // Render the scene
        engine.render(camera, WIN_WIDTH, WIN_HEIGHT);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
