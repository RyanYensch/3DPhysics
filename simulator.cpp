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
    // Initialising the window
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, NULL, NULL);
    if (!window) return -1;
    glfwMakeContextCurrent(window);


    // Creating The Engine
    PhysicsEngine engine;
    RigidBody ball = {glm::vec3(0, 10, 0), glm::vec3(0, 0, 0), glm::vec3(0, -9.81, 0), 1.0f};
    engine.addObject(ball);


    // Update Loop
    while (!glfwWindowShouldClose(window)) {
        engine.update(TICK_RATE);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}