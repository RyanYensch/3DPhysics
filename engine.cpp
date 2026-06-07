#include "engine.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <cmath>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


void PhysicsEngine::addRigidObject(const RigidBody& obj) {
    rigidObjects.push_back(obj);
}

void PhysicsEngine::addSimpleObject(const SimpleBody& obj) {
    simpleObjects.push_back(obj);
}

void PhysicsEngine::update(float deltaTime) {
    for (auto& obj : rigidObjects) {
        if (obj.mass == 0.0f) continue;

        // Linear Integration
        obj.velocity += obj.acceleration * deltaTime;
        obj.velocity.y -= gravity * deltaTime;
        obj.position += obj.velocity * deltaTime;

        // Angular Integration
        obj.angularVelocity.x += (obj.torque.x / obj.localInertia.x) * deltaTime;
        obj.angularVelocity.y += (obj.torque.y / obj.localInertia.y) * deltaTime;
        obj.angularVelocity.z += (obj.torque.z / obj.localInertia.z) * deltaTime;

        // Convert angular velocity to quaternion rate of change
        glm::quat spin(0.0f, obj.angularVelocity.x, obj.angularVelocity.y, obj.angularVelocity.z);

        // Add spin to orientation
        obj.orientation += spin * obj.orientation * (0.5f * deltaTime);
        obj.orientation = glm::normalize(obj.orientation);

        obj.torque = glm::vec3(0.0f);
    }

    for (size_t i{0}; i < rigidObjects.size(); ++i) {
        auto& objA = rigidObjects[i];

        for (size_t j{i + 1}; j < rigidObjects.size(); j++) {
            auto& objB = rigidObjects[j];

            // both static
            if (objA.mass == 0.0f && objB.mass == 0.0f) continue;

            // Rotate resistance to spin
            glm::mat3 rotA = glm::mat3_cast(objA.orientation);
            glm::mat3 invInertiaLocalA = glm::mat3(0.0f);
            if (objA.mass > 0.0f) {
                invInertiaLocalA[0][0] = 1.0f / objA.localInertia.x;
                invInertiaLocalA[1][1] = 1.0f / objA.localInertia.y;
                invInertiaLocalA[2][2] = 1.0f / objA.localInertia.z;
            }
            glm::mat3 invInteriaWorldA = rotA * invInertiaLocalA * glm::transpose(rotA);

            glm::mat3 rotB = glm::mat3_cast(objB.orientation);
            glm::mat3 invInertiaLocalB = glm::mat3(0.0f);
            if (objB.mass > 0.0f) {
                invInertiaLocalB[0][0] = 1.0f / objB.localInertia.x;
                invInertiaLocalB[1][1] = 1.0f / objB.localInertia.y;
                invInertiaLocalB[2][2] = 1.0f / objB.localInertia.z;
            }
            glm::mat3 invInteriaWorldB = rotB * invInertiaLocalB * glm::transpose(rotB);


            // Check if any corner A is in B
            for (const auto& localVertA : objA.verticies) {
                // Get exact world coords of A corner
                glm::vec3 worldVertA = objA.position + (objA.orientation * (localVertA * objA.scale));

                // Transform A corner to B local
                glm::vec3 vertInB = glm::inverse(objB.orientation) * (worldVertA - objB.position);

                // Is A in B
                glm::vec3 halfB = objB.scale / 2.0f;

                if (std::abs(vertInB.x) < halfB.x &&
                    std::abs(vertInB.y) < halfB.y &&
                    std::abs(vertInB.z) < halfB.z) {

                    // What face is overlapping
                    float overlapX = halfB.x - std::abs(vertInB.x);
                    float overlapY = halfB.y - std::abs(vertInB.y);
                    float overlapZ = halfB.z - std::abs(vertInB.z);

                    glm::vec3 localNorm(0.0f);
                    float minOverlap = std::min({overlapX, overlapY, overlapZ});

                    if (minOverlap == overlapX) {
                        localNorm = glm::vec3(vertInB.x > 0 ? 1.0f : -1.0f, 0.0f, 0.0f);
                    } else if (minOverlap == overlapY) {
                        localNorm = glm::vec3(0.0f, vertInB.y > 0 ? 1.0f : -1.0f, 0.0f);
                    } else {
                        localNorm = glm::vec3(0.0f, 0.0f, vertInB.z > 0 ? 1.0f : -1.0f);
                    }

                    // Conver Back to world
                    glm::vec3 worldNorm = objB.orientation * localNorm;

                    // Calculate lever arms from center of mass to contact
                    glm::vec3 rA = worldVertA - objA.position;
                    glm::vec3 rB = worldVertA - objB.position;

                    // Calculate exact velocity at the contact point
                    glm::vec3 vA = objA.velocity + glm::cross(objA.angularVelocity, rA);
                    glm::vec3 vB = objB.velocity + glm::cross(objB.angularVelocity, rB);
                    glm::vec3 relVel = vA - vB;

                    float velAlongNorm = glm::dot(relVel, worldNorm);
                    if (velAlongNorm > 0.0f) continue; // Already bouncing apart

                    // Cofficient of restitution
                    // 0.0f stops, 1.0f super bouncy
                    float e = (std::abs(velAlongNorm) < 0.2f) ? 0.0f : 0.5f;

                    // inverse mass
                    float invMassA = (objA.mass > 0.0f) ? 1.0f / objA.mass : 0.0f;
                    float invMassB = (objB.mass > 0.0f) ? 1.0f / objB.mass : 0.0f;

                    // Calculate how muhc the rotation absorbs impact
                    glm::vec3 angularFactA = glm::cross(invInteriaWorldA * glm::cross(rA, worldNorm), rA);
                    glm::vec3 angularFactB = glm::cross(invInteriaWorldB * glm::cross(rB, worldNorm), rB);
                    float angularEffect = glm::dot(angularFactA + angularFactB, worldNorm);

                    // Magnitude of the push
                    float j = -(1.0f + e) * velAlongNorm;
                    j /= (invMassA + invMassB + angularEffect);

                    glm::vec3 impulse = j * worldNorm;

                    // Apply Linear
                    if (objA.mass > 0.0f) objA.velocity += impulse * invMassA;
                    if (objB.mass > 0.0f) objB.velocity -= impulse * invMassB;

                    // Apply Angular
                    if (objA.mass > 0.0f) objA.angularVelocity += invInteriaWorldA * glm::cross(rA, impulse);
                    if (objB.mass > 0.0f) objB.angularVelocity -= invInteriaWorldB * glm::cross(rB, impulse);

                    // Correction
                    const float percent = 0.8f; // percent per frame
                    const float slop = 0.01f; // buffer
                    glm::vec3 correction = (std::max(minOverlap - slop, 0.0f) / (invMassA + invMassB)) * percent * worldNorm;

                    if (objA.mass > 0.0f) objA.position += correction * invMassA;
                    if (objB.mass > 0.0f) objB.position -= correction * invMassB;
                }
            }

            // Check if B's Corners are in A
            for (const auto& localVertB : objB.verticies) {
                glm::vec3 worldVertB = objB.position + (objB.orientation * (localVertB * objB.scale));
                glm::vec3 vertInA = glm::inverse(objA.orientation) * (worldVertB - objA.position);
                glm::vec3 halfA = objA.scale / 2.0f;

                if (std::abs(vertInA.x) < halfA.x && std::abs(vertInA.y) < halfA.y && std::abs(vertInA.z) < halfA.z) {
                    float overlapX = halfA.x - std::abs(vertInA.x);
                    float overlapY = halfA.y - std::abs(vertInA.y);
                    float overlapZ = halfA.z - std::abs(vertInA.z);

                    glm::vec3 localNorm(0.0f);
                    float minOverlap = std::min({overlapX, overlapY, overlapZ});
                    if (minOverlap == overlapX) localNorm = glm::vec3(vertInA.x > 0 ? 1.0f : -1.0f, 0.0f, 0.0f);
                    else if (minOverlap == overlapY) localNorm = glm::vec3(0.0f, vertInA.y > 0 ? 1.0f : -1.0f, 0.0f);
                    else localNorm = glm::vec3(0.0f, 0.0f, vertInA.z > 0 ? 1.0f : -1.0f);

                    glm::vec3 worldNorm = -(objA.orientation * localNorm);


                    glm::vec3 rA = worldVertB - objA.position;
                    glm::vec3 rB = worldVertB - objB.position;

                    glm::vec3 vA = objA.velocity + glm::cross(objA.angularVelocity, rA);
                    glm::vec3 vB = objB.velocity + glm::cross(objB.angularVelocity, rB);
                    glm::vec3 relVel = vA - vB;

                    float velAlongNorm = glm::dot(relVel, worldNorm);
                    if (velAlongNorm > 0.0f) continue; 

                    float e = (std::abs(velAlongNorm) < 0.2f) ? 0.0f : 0.5f;

                    float invMassA = (objA.mass > 0.0f) ? 1.0f / objA.mass : 0.0f;
                    float invMassB = (objB.mass > 0.0f) ? 1.0f / objB.mass : 0.0f;

                    glm::vec3 angularFactA = glm::cross(invInteriaWorldA * glm::cross(rA, worldNorm), rA);
                    glm::vec3 angularFactB = glm::cross(invInteriaWorldB * glm::cross(rB, worldNorm), rB);
                    float angularEffect = glm::dot(angularFactA + angularFactB, worldNorm);

                    float j = -(1.0f + e) * velAlongNorm;
                    j /= (invMassA + invMassB + angularEffect);

                    glm::vec3 impulse = j * worldNorm;

                    if (objA.mass > 0.0f) objA.velocity += impulse * invMassA;
                    if (objB.mass > 0.0f) objB.velocity -= impulse * invMassB;

                    if (objA.mass > 0.0f) objA.angularVelocity += invInteriaWorldA * glm::cross(rA, impulse);
                    if (objB.mass > 0.0f) objB.angularVelocity -= invInteriaWorldB * glm::cross(rB, impulse);

                    const float percent = 0.8f; 
                    const float slop = 0.01f; 
                    glm::vec3 correction = (std::max(minOverlap - slop, 0.0f) / (invMassA + invMassB)) * percent * worldNorm;

                    if (objA.mass > 0.0f) objA.position += correction * invMassA;
                    if (objB.mass > 0.0f) objB.position -= correction * invMassB;
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

        model *= glm::mat4_cast(obj.orientation);

        model = glm::scale(model, obj.scale);

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

void PhysicsEngine::render(const Camera& camera, int windowWidth, int windowHeight, bool isPaused, bool isSummoning, const RigidBody* previewObject) {
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
    glEnable(GL_NORMALIZE);
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

    if (isSummoning && previewObject != nullptr) {
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        std::vector<RigidBody> previewVec = { *previewObject };
        renderObjects(previewVec, view);

        if (glm::length(previewObject->velocity) > 0.0f) {
            glLoadMatrixf(glm::value_ptr(view));

            glColor3f(1.0f, 1.0f, 0.0f);
            glLineWidth(3.0f);

            glBegin(GL_LINES);
                glVertex3f(previewObject->position.x, previewObject->position.y, previewObject->position.z);

                glm::vec3 end = previewObject->position + previewObject->velocity;
                glVertex3f(end.x, end.y, end.z);
            glEnd();

            glLineWidth(1.0f);
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_LIGHTING);
    }


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

