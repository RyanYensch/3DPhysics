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
        for (size_t i{0}; i < obj.verticies.size(); ++i) {
            glNormal3f(obj.normals[i].x, obj.normals[i].y, obj.normals[i].z);
            glVertex3f(obj.verticies[i].x,obj.verticies[i].y,obj.verticies[i].z);
        }
        glEnd();
    }
}

void PhysicsEngine::render(const Camera& camera, int windowWidth, int windowHeight, bool isPaused) {
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

    // Lighting
    glLoadMatrixf(glm::value_ptr(view));

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // light up high and right forward
    GLfloat lightPos[] = {10.0f, 20.0f, 10.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // ambient light for non pitch black shadows
    GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

    // Render each object
    renderObjects(rigidObjects, view);
    renderObjects(simpleObjects, view);


    if (isPaused) {
        glDisable(GL_LIGHTING);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        glOrtho(0, windowWidth, windowHeight, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glDisable(GL_DEPTH_TEST);

        glColor3f(1.0f, 1.0f, 1.0f);
        float iconX = windowWidth - 50.0f;
        float iconY = 20.0f;
        float barWidth = 8.0f;
        float barHeight = 35.0f;
        float spacing = 12.0f;

        glBegin(GL_QUADS);
            // Left Vertical Bar
            glVertex2f(iconX, iconY);
            glVertex2f(iconX + barWidth, iconY);
            glVertex2f(iconX + barWidth, iconY + barHeight);
            glVertex2f(iconX, iconY + barHeight);

            // Right Vertical Bar
            glVertex2f(iconX + barWidth + spacing, iconY);
            glVertex2f(iconX + barWidth + spacing + barWidth, iconY);
            glVertex2f(iconX + barWidth + spacing + barWidth, iconY + barHeight);
            glVertex2f(iconX + barWidth + spacing, iconY + barHeight);
        glEnd();

        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }
}

