#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "engine.h"
#include "objects.h"
#include "camera.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


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

    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        cameraInstance->rotateCamera(xOffset * cameraInstance->sensitivity, yOffset * cameraInstance->sensitivity);
    }
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

    // Create Camera (Positioned initially to view the cube)
    Camera camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 60.0f, 0.1f, 100.0f, 1.5f, 0.1f);

    // scrollwheel actions for fov
    glfwSetWindowUserPointer(window, &camera);
    glfwSetScrollCallback(window, scroll_callback);

    // mouse actions for rotation
    glfwSetCursorPosCallback(window, mouse_callback);

    // update for when screen size changes
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Setup IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");


    // Create the engine and add a rigid body (the cube)
    PhysicsEngine engine;
    RigidBody cubeA = {glm::vec3(-4.0f, 5.0f, -5.0f), glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,1.0f,1.0f), ShapeType::Cuboid, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f};
    cubeA.angularVelocity = glm::vec3(2.0f, 5.0f, 1.0f);
    engine.addRigidObject(cubeA);

    RigidBody cubeB = {glm::vec3(4.0f, -1.0f, -5.0f), glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,1.0f,1.0f), ShapeType::Cuboid, glm::vec3(0.0f, 0.0f, 1.0f), 1.0f};
    engine.addRigidObject(cubeB);

    RigidBody floor = {glm::vec3(0.0f, -2.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f,1.0f,10.0f), ShapeType::Cuboid, glm::vec3(0.3f, 0.3f, 0.3f), 0.0f};
    engine.addRigidObject(floor);


    // Hide and lock cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // The timing rates of the function
    float prevTime = glfwGetTime();
    float deltaTime = 0.0f;

    // Pause state
    bool isPaused = false;
    bool pauseKeyPressed = false;

    // Summoning state
    bool isSummoning = false;
    bool tabKeyPressed = false;
    RigidBody previewObject = {
        glm::vec3(0.0f, 5.0f, -5.0f), glm::vec3(0.0f), glm::vec3(0.0f),
        glm::vec3(1.0f), ShapeType::Cuboid, glm::vec3(0.0f, 1.0f, 0.0f), 1.0f
    };

    // Main update loop
    while (!glfwWindowShouldClose(window)) {
        //update objects
        double currTime = glfwGetTime();
        deltaTime = currTime - prevTime;
        prevTime = currTime;

        // New IMGUI frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            if (!pauseKeyPressed) {
                isPaused = !isPaused;
                pauseKeyPressed = true;
            }
        } else {
            pauseKeyPressed = false;
        }

        // Toggle summoning
        if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
            if (!tabKeyPressed) {
                isSummoning = !isSummoning;
                tabKeyPressed = true;

                if (isSummoning) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                else {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    camera.firstMouse = true;
                }
            }
        } else {
            tabKeyPressed = false;
        }

        // Draw the UI
        if (isSummoning) {
            ImGui::Begin("Summon Object", &isSummoning, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::SliderFloat3("Position", glm::value_ptr(previewObject.position), -10.0f, 10.0f);
            ImGui::SliderFloat3("Velocity", glm::value_ptr(previewObject.velocity), -5.0f, 5.0f);
            ImGui::SliderFloat3("Scale", glm::value_ptr(previewObject.scale), 0.1f, 5.0f);
            ImGui::ColorEdit3("Colour", glm::value_ptr(previewObject.colour));
            ImGui::SliderFloat("Mass", &previewObject.mass, 0.0f, 50.0f);

            if (ImGui::Button("Summon Into World!", ImVec2(-1.0f, 40.0f))) {
                engine.addRigidObject(previewObject);
                isSummoning = false;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    camera.firstMouse = true;
            }

            ImGui::End();
        }

        if (!isPaused) {
            engine.update(deltaTime);
        }

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

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !io.WantCaptureMouse) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            camera.firstMouse = true;
        }

        // Render the scene (objects will stay still, only the camera moves)
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        engine.render(camera, width, height, isPaused, isSummoning, &previewObject);

        // Render IMGUI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();


    glfwTerminate();
    return 0;
}
