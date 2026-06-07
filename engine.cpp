#include "engine.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void PhysicsEngine::addRigidObject(const RigidBody& obj) {
    rigidObjects.push_back(obj);
}

void PhysicsEngine::addSimpleObject(const SimpleBody& obj) {
    simpleObjects.push_back(obj);
}

void PhysicsEngine::update(float deltaTime) {
    for (size_t i{0}; i < rigidObjects.size(); ++i) {
        auto& obj = rigidObjects[i];

        if (obj.mass == 0.0f) continue;

        obj.velocity += obj.acceleration * deltaTime;
        obj.velocity.y -= gravity * deltaTime;
        obj.position += obj.velocity * deltaTime;

        for (size_t j{0}; j < rigidObjects.size(); j++) {
            if (i == j) continue;

            auto &other = rigidObjects[j];

            bool collisionX = obj.position.x + obj.scale.x / 2.0f > other.position.x - other.scale.x / 2.0f &&
                              obj.position.x - obj.scale.x / 2.0f < other.position.x + other.scale.x / 2.0f;

            bool collisionY = obj.position.y + obj.scale.y / 2.0f > other.position.y - other.scale.y / 2.0f &&
                              obj.position.y - obj.scale.y / 2.0f < other.position.y + other.scale.y / 2.0f;

            bool collisionZ = obj.position.z + obj.scale.z / 2.0f > other.position.z - other.scale.z / 2.0f &&
                              obj.position.z - obj.scale.z / 2.0f < other.position.z + other.scale.z / 2.0f;

            if (collisionX && collisionY && collisionZ) {
                if (obj.velocity.y < 0.0f) {
                    obj.position.y = other.position.y + (other.scale.y / 2.0f) + (obj.scale.y / 2.0f);
                    obj.velocity.y = 0.0f;
                }
            }
        }
    }
}

template <typename T>
void PhysicsEngine::renderObjects(const std::vector<T>& objects, glm::mat4 view) {
    for (const auto& obj : objects) {
        // Create the model matrix to translate the object to its correct position
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, obj.position);

        // Combine the model, view, and projection matrices
        glm::mat4 modelView = view * model;

        // Load the model-view-projection matrix into OpenGL
        glLoadMatrixf(glm::value_ptr(modelView));

        // Draw the cube
        glBegin(GL_QUADS);
        glColor3f(obj.colour[0], obj.colour[1], obj.colour[2]);
        for (const auto& v : obj.verticies) {
            glVertex3f(v.x,v.y,v.z);
        }
        glEnd();
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

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection));

    glMatrixMode(GL_MODELVIEW);

    // Render each object
    renderObjects(rigidObjects, view);
    renderObjects(simpleObjects, view);
}

