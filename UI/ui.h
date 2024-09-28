#include <iostream>
#include "System.h"

class UI : public System {

public:

    UI() {

    }
    

    void AddEntity(int entityId) override {
        std::cout << "Added entity " << entityId << " to UI System." << std::endl;
    }

    const char* GetName() const override {
        return "UI";
    }

    void RemoveEntity(int entityId) override {
        std::cout << "Removed entity " << entityId << " from UI System." << std::endl;
    }

};