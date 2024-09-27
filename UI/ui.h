#include <iostream>
#include "System.h"

class UI : System {

public:

    UI() {

    }

    void Update(float deltaTime) override {
        std::cout << "Updating UI" << std::endl;
    }

    void FixedUpdate(float fixedDeltaTime) override {
        std::cout << "Running Fixed Update for UI" << std::endl;
    }

    void AddEntity(int entityId) override {
        std::cout << "Added entity " << entityId << " to UI System." << std::endl;
    }

    void RemoveEntity(int entityId) override {
        std::cout << "Removed entity " << entityId << " from UI System." << std::endl;
    }

};