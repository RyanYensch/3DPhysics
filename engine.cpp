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
        obj.velocity.y -= gravity * deltaTime;
        obj.position += obj.velocity * deltaTime;
    }
}

void PhysicsEngine::render(const Camera& camera, int windowWidth, int windowHeight) {
    // Clear the screen
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set up the projection and view
    glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)windowWidth / windowHeight, camera.nearPlane, camera.farPlane);
    glm::mat4 view = camera.getViewMatrix();

    // Combine the projection and view matrices
    glm::mat4 projectionView = projection * view;

    // Render each object
    for (const auto& obj : objects) {
        // Create the model matrix to translate the object to its correct position
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, obj.position);

        // Combine the model, view, and projection matrices
        glm::mat4 mvp = projectionView * model;

        // Load the model-view-projection matrix into OpenGL
        glLoadMatrixf(glm::value_ptr(mvp));

        // Draw the cube
        glBegin(GL_QUADS);

        glColor3f(obj.colour[0], obj.colour[1], obj.colour[2]);

        for (const auto& v : obj.verticies) {
            glVertex3f(v.x,v.y,v.z);
        }

        glEnd();
    }
}

