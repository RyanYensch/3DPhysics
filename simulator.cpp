#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "engine.h"


#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define WIN_TITLE "3D Engine"
#define FPS 60
#define TICK_RATE  1f / FPS

int main() {
    // Initialising the window
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, NULL, NULL);
    if (!window) return -1;
    glfwMakeContextCurrent(window);



    // Update Loop
    while (!glfwWindowShouldClose(window)) {
    }


    glfwTerminate();
    return 0;
}