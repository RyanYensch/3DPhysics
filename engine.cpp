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
    // Clear Screen
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
    glClear(GL_COLOR_BUFFER_BIT);

    // enable depth
    glEnable(GL_DEPTH_TEST);

    // set up perspective and view
    glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)windowWidth / windowHeight, camera.nearPlane, camera.farPlane);
    glm::mat4 view = camera.getViewMatrix();

    // render objects
    for (const auto& obj : objects) {
        // create the model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, obj.position);

        // combine model view and projection
        glm::mat4 mvp = model * view * projection;

        //draw cube
        glLoadMatrixf(glm::value_ptr(mvp));

        glBegin(GL_QUADS);

        //front
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f,  0.5f);
        glVertex3f( 0.5f, -0.5f,  0.5f);
        glVertex3f( 0.5f,  0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f,  0.5f);

        //back
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f,  0.5f, -0.5f);
        glVertex3f(-0.5f,  0.5f, -0.5f);

        //left
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f, -0.5f);

        //right
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f( 0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f,  0.5f);
        glVertex3f( 0.5f,  0.5f,  0.5f);
        glVertex3f( 0.5f,  0.5f, -0.5f);

        glEnd();
    }
}
