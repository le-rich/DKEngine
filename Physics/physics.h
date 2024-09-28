#include <iostream>
#include "System.h"
#include "Managers\EntityManager.h"

class Physics : public System {

public:

	Physics() {

	}

    void AddEntity(int entityId) override {
        std::cout << "Added entity " << entityId << " to Physics System." << std::endl;
    }

    const char* GetName() const override {
        return "Physics";
    }

    void RemoveEntity(int entityId) override {
        std::cout << "Removed entity " << entityId << " from Physics System." << std::endl;
    }

};