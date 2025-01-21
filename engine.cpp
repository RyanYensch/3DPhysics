#include "engine.h"
#include <glad/glad.h>

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
    // Calculation Matricies
    glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)windowWidth / windowHeight, 0.1f, 100f);
    glm::mat4 view = camera.getViewMatrix();

    //clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render the objects
    for (const auto& obj : objects) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, obj.position);

        glm::mat4 mvp = projection * view * model;
    }

}
