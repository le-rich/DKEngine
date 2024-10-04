#include <iostream>
#include "System.h"
#include "Managers\EntityManager.h"

class Physics : public System {

public:

	Physics() {

	}

    const char* GetName() const override {
        return "Physics";
    }

    void Update() override {
        // Update Loop logic here

    }

    void FixedUpdate() override {
        // Update Loop logic here

    }


};