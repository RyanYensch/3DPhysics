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
    RigidBody cube = {glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -0.1f, 0.0f), 1.0f};
    engine.addObject(cube);

    // Create Camera (Positioned initially to view the cube)
    Camera camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f, 0.1f, 100.0f, 0.1f);

    // Main update loop
    while (!glfwWindowShouldClose(window)) {
        // Handle camera movement
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.position += camera.speed * camera.front;  // Move camera forward
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.position -= camera.speed * camera.front;  // Move camera backward
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * camera.speed; // Move camera left
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * camera.speed; // Move camera right
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            camera.position.y += camera.speed; // Move camera up
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            camera.position.y -= camera.speed; // Move camera down
        }

        // Update the view matrix after the camera position change
        glm::mat4 view = camera.getViewMatrix();

        // Render the scene (cube will stay still, only the camera moves)
        engine.render(camera, WIN_WIDTH, WIN_HEIGHT);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}
