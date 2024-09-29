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


};