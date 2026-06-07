#include "engine.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <cmath>

void PhysicsEngine::addRigidObject(const RigidBody& obj) {
    rigidObjects.push_back(obj);
}

void PhysicsEngine::addSimpleObject(const SimpleBody& obj) {
    simpleObjects.push_back(obj);
}

void PhysicsEngine::update(float deltaTime) {
    for (auto& obj : rigidObjects) {
        if (obj.mass == 0.0f) continue;

        obj.velocity += obj.acceleration * deltaTime;
        obj.velocity.y -= gravity * deltaTime;
        obj.position += obj.velocity * deltaTime;
    }

    for (size_t i{0}; i < rigidObjects.size(); ++i) {
        auto& objA = rigidObjects[i];

        for (size_t j{i + 1}; j < rigidObjects.size(); j++) {
            auto& objB = rigidObjects[j];

            // both static
            if (objA.mass == 0.0f && objB.mass == 0.0f) continue;

            glm::vec3 halfA = objA.scale / 2.0f;
            glm::vec3 halfB = objB.scale / 2.0f;
            glm::vec3 dist = objA.position - objB.position;

            float overlapX = (halfA.x + halfB.x) - std::abs(dist.x);
            float overlapY = (halfA.y + halfB.y) - std::abs(dist.y);
            float overlapZ = (halfA.z + halfB.z) - std::abs(dist.z);

            // if overlapping it is a collision
            if (overlapX > 0 && overlapY > 0 && overlapZ > 0) {
                // collision normal is smallest overlap
                glm::vec3 normal(0.0f);
                float minOverlap = std::min({overlapX, overlapY, overlapZ});

                if (minOverlap == overlapX) {
                    normal = glm::vec3(dist.x > 0 ? 1.0f : -1.0f, 0.0f, 0.0f);
                } else if (minOverlap == overlapY) {
                    normal = glm::vec3(0.0f, dist.y > 0 ? 1.0f : -1.0f, 0.0f);
                } else {
                    normal = glm::vec3(0.0f, 0.0f, dist.z > 0 ? 1.0f : -1.0f);
                }

                // inverse mass
                float invMassA = (objA.mass > 0.0f) ? 1.0f / objA.mass : 0.0f;
                float invMassB = (objB.mass > 0.0f) ? 1.0f / objB.mass : 0.0f;
                float sumInvMass = invMassA + invMassB;

                // correct position
                const float percent = 0.8f; // percent per frame
                const float slop = 0.01f; // buffer
                glm::vec3 correction = (std::max(minOverlap - slop, 0.0f) / sumInvMass) * percent * normal;

                if (objA.mass > 0.0f) objA.position += correction * invMassA;
                if (objB.mass > 0.0f) objB.position -= correction * invMassB;


                // Momentum between objects
                glm::vec3 relVel = objA.velocity - objB.velocity;

                // How fast moving towards eachother
                float velAlongNorm = glm::dot(relVel, normal);

                // Already moving apart
                if (velAlongNorm > 0.0f) continue;

                // Cofficient of restitution
                // 0.0f stops, 1.0f super bouncy
                float e = 0.5f;

                float j = -(1.0 + e) * velAlongNorm;
                j /= sumInvMass;

                glm::vec3 impluse = j * normal;
                if (objA.mass > 0.0f) objA.velocity += impluse * invMassA;
                if (objB.mass > 0.0f) objB.velocity -= impluse * invMassB;
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

