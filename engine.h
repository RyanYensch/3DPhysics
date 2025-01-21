#ifndef ENGINE_H
#define ENGINE_H

struct RigidBody;

class PhysicsEngine {
public:
    void addObject(const RigidBody& obj);
    void update(float deltaTime);
};


#endif // ENGINE_H