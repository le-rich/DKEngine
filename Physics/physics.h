#include <iostream>
#include "System.h"
#include "Managers\EntityManager.h"

class Physics : System {

public:

	Physics() {

	}

    void Update(float deltaTime) override {
        std::cout << "Updating Physics" << std::endl;
    }

    void FixedUpdate(float fixedDeltaTime) override {
        std::cout << "Running Fixed Update for Physics" << std::endl;
    }

    void AddEntity(int entityId) override {
        std::cout << "Added entity " << entityId << " to Physics System." << std::endl;
    }

    void RemoveEntity(int entityId) override {
        std::cout << "Removed entity " << entityId << " from Physics System." << std::endl;
    }

};