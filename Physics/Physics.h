#include <iostream>
#include "System.h"
#include "Managers\EntityManager.h"
#include "../Physics/include/world.h"
#include "glm.hpp"
#include <mutex>
#include <chrono>
#include "Components/TransformComponent.h"
#include <Components/RigidBodyComponent.h>


// 120 HZ tick rate
std::chrono::milliseconds PHYSICS_TICK_RATE = std::chrono::milliseconds(8);

class Physics : public System {
public:
    AE86::World world;
    std::chrono::time_point<std::chrono::high_resolution_clock> prevTime;
    std::chrono::duration<double> timeBuffer;
    AE86::RigidBody* body;
    TransformComponent* CAR_TRANSFORM;
    std::mutex mtx;
    
	Physics(TransformComponent* carTransform, AE86::RigidBody* rbody) {
        CAR_TRANSFORM = carTransform;
        world = AE86::World();
        prevTime = std::chrono::high_resolution_clock::now();
        timeBuffer = std::chrono::milliseconds(0);
        body = rbody;
        body->setAwake(true);
        body->setInverseMass(0.95f);
        body->setLinearDamping(0.50f);
        body->setAngularDamping(0.50f);

        glm::quat initOrientation = CAR_TRANSFORM->getLocalOrientation();
        glm::vec3 initPosition = CAR_TRANSFORM->getLocalPosition();

        body->setOrientation(AE86::Quaternion(initOrientation.w, initOrientation.x, initOrientation.y, initOrientation.z));
        body->setPosition(AE86::Vector3(initPosition.x, initPosition.y, initPosition.z));
        body->calculateDerivedData();

        world.addRigidBody(body);
        world.startFrame();
	}

    const char* GetName() const override {
        return "Physics";
    }

    void Update(float deltaTime) override {
        // Update Loop logic here
        //auto currentTime = std::chrono::high_resolution_clock::now();
        //timeBuffer += currentTime - prevTime;
        //prevTime = currentTime;

        //if (timeBuffer >= PHYSICS_TICK_RATE) {
        //    world.runPhysics((AE86::real) timeBuffer.count() / 1000.0f);
        //}
    }

    void FixedUpdate() override {
        // Update Loop logic here

        // grab all rigidbody components and update internal locations to transforms.
        // TODO: possible race conditions, need to lock scene graph here until all
        // rb->updates are done.
        ComponentMask rigidBodyComponentMask = rigidBodyComponentMask.set(static_cast<size_t>(ComponentType::RigidBody));
        std::vector<Entity*> rigidBodyEntities = EntityManager::getInstance().findEntitiesByComponentMask(rigidBodyComponentMask);

        for (Entity* rigidBodyEntity : rigidBodyEntities) {
            RigidBodyComponent* rigidBodyComponent = dynamic_cast<RigidBodyComponent*>(
                rigidBodyEntity->getComponent(ComponentType::RigidBody)
            );
            rigidBodyComponent->update();
        }

        //std::cout << "PHYSICS - WE ARE GRABBING LOCK FOR CAR\n";
        // std::lock_guard<std::mutex> lck(CAR_TRANSFORM->mtx);
        world.runPhysics(0.02);

        //std::cout << "PHYSICS - WE HAVE DONE A PHYSICS TIME-STEP\n";

        AE86::Vector3 updatedPosition = body->getPosition();
        AE86::Quaternion updatedOrientation = body->getOrientation();

        //std::cout << "PHYSICS - RB POSITION: " << updatedPosition.x << ", " << updatedPosition.y << ", " << updatedPosition.z << "\n";


        CAR_TRANSFORM->setLocalPosition(glm::vec3(updatedPosition.x, updatedPosition.y, updatedPosition.z));
        CAR_TRANSFORM->setLocalOrientation(glm::quat(updatedOrientation.r, updatedOrientation.i, updatedOrientation.j, updatedOrientation.k));
        //std::cout << "PHYSICS - WE HAVE UPDATED THE TRANSFORM AND RELEASING LOCK\n";
    }
};