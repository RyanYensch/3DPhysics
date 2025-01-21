#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "engine.h"


#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define WIN_TITLE "3D Engine"
#define FPS 60
#define TICK_RATE  0.016f

int main() {
    // Initialize GLFW
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, NULL, NULL);
    if (!window) return -1;
    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST); // Enable depth testing
    
    // Enable V-Sync
    glfwSwapInterval(1);

    // Setting up viewport
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    

    // Creating The Engine
    PhysicsEngine engine;
    RigidBody ball = {glm::vec3(0, 10, 0), glm::vec3(0, 0, 0), glm::vec3(0, -9.81, 0), 1.0f};
    engine.addObject(ball);

    // Create Camera
    Camera camera(glm::vec3(0.0f, 5.0f, 15.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f, 0.1f, 100.0f);


    // Update Loop
    while (!glfwWindowShouldClose(window)) {
        engine.update(TICK_RATE);

        // handle camera movement
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


        // Render
        engine.render(camera, WIN_WIDTH, WIN_HEIGHT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}