#include <iostream>
#include "System.h"

class UI : public System {

public:

    UI() {

    }
    
    const char* GetName() const override {
        return "UI";
    }

    void Update() override {
        // Update Loop logic here
        std::cout << "UI Update" << std::endl;

    }

    void FixedUpdate() override {
        // Update Loop logic here

    }


};