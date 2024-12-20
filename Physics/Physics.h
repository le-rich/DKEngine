#pragma once

#include <iostream>
#include "System.h"
#include "Managers\EntityManager.h"
#include "../Physics/include/world.h"
#include "glm.hpp"
#include <mutex>
#include <chrono>
#include "Components/TransformComponent.h"
#include <Components/RigidBodyComponent.h>


// tick rate in ms, 8ms = 120hz
static float PHYSICS_UPDATE_INTERVAL = 8;

class Physics : public System {
public:
    AE86::World world;
    std::mutex mtx;

    Physics() {
        world = AE86::World();
        world.startFrame();
    }

    const char* GetName() const override {
        return "Physics";
    }

    void Update(float deltaTime) override {
    }

    void FixedUpdate() override {
        // TODO: possible race conditions, need to lock scene graph here until all
        // rb->updates are done.
        std::vector<AE86::RigidBody*> rigidBodies;
        std::vector<RigidBodyComponent*> components;

        std::vector<UUIDv4::UUID> rigidBodyEntitiesUUIDs = EntityManager::getInstance().findEntitiesByComponent(ComponentType::RigidBody);
        for (UUIDv4::UUID rigidBodyEntityUUID : rigidBodyEntitiesUUIDs) {
            Entity* rigidBodyEntity = EntityManager::getInstance().getEntity(rigidBodyEntityUUID);
            RigidBodyComponent* rigidBodyComponent = dynamic_cast<RigidBodyComponent*>(
                rigidBodyEntity->getComponent(ComponentType::RigidBody)
            );
            rigidBodyComponent->update();
            AE86::RigidBody* rigidBody = rigidBodyComponent->getRigidBody().get();
            rigidBodies.emplace_back(rigidBody);
            components.emplace_back(rigidBodyComponent);
        }

        world.integrate(rigidBodies, PHYSICS_UPDATE_INTERVAL / 1000.0f); // TODO: remove magic constant

        // update all transform positions after physics time-step
        for (RigidBodyComponent* rigidBodyComponent : components) {
            auto body = rigidBodyComponent->getRigidBody();
            AE86::Vector3 updatedPosition = body->getPosition();
            AE86::Quaternion updatedOrientation = body->getOrientation();

            TransformComponent* transform = rigidBodyComponent->entity->transform;
            transform->setLocalPosition(glm::vec3(updatedPosition.x, updatedPosition.y, updatedPosition.z));
            transform->setLocalOrientation(glm::quat(updatedOrientation.r, updatedOrientation.i, updatedOrientation.j, updatedOrientation.k));
        }
    }
};