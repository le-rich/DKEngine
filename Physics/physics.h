#include <iostream>
#include "System.h"
#include "Managers\EntityManager.h"
#include "../Physics/include/world.h"
#include <chrono>


// 120 HZ tick rate
std::chrono::milliseconds PHYSICS_TICK_RATE = std::chrono::milliseconds(8);

class Physics : public System {
public:
    AE86::World world;
    std::chrono::time_point<std::chrono::high_resolution_clock> prevTime;
    std::chrono::duration<double> timeBuffer;
    
	Physics() {
        world = AE86::World();
        prevTime = std::chrono::high_resolution_clock::now();
        timeBuffer = std::chrono::milliseconds(0);
        world.startFrame();
	}

    const char* GetName() const override {
        return "Physics";
    }

    void Update() override {
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

        world.runPhysics(0.016);
    }


};