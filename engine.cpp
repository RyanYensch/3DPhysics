#include "engine.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void PhysicsEngine::addObject(const RigidBody& obj) {
    objects.push_back(obj);
}

void PhysicsEngine::update(float deltaTime) {
    for (auto& obj : objects) {
        obj.velocity += obj.acceleration * deltaTime;
        obj.position += obj.velocity * deltaTime;
    }
}

void PhysicsEngine::render(const Camera& camera, int windowWidth, int windowHeight) {
    // Clear Screen with a dark gray color
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set up the projection matrix (field of view, aspect ratio, near/far planes)
    glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)windowWidth / windowHeight, camera.nearPlane, camera.farPlane);

    // View matrix (camera's position and orientation)
    glm::mat4 view = camera.getViewMatrix();

    // Render all objects
    for (const auto& obj : objects) {
        // Model matrix (object's position, rotation, scale)
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, obj.position); // Translate the cube to its position

        // MVP matrix (Model-View-Projection)
        glm::mat4 mvp = projection * view * model;

        glBegin(GL_QUADS);
        // Front face (red)
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f,  0.5f);
        glVertex3f( 0.5f, -0.5f,  0.5f);
        glVertex3f( 0.5f,  0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f,  0.5f);

        // Back face (green)
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f,  0.5f, -0.5f);
        glVertex3f(-0.5f,  0.5f, -0.5f);

        // Left face (blue)
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f, -0.5f);

        // Right face (yellow)
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f( 0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f,  0.5f);
        glVertex3f( 0.5f,  0.5f,  0.5f);
        glVertex3f( 0.5f,  0.5f, -0.5f);
        glEnd();
    }
}
