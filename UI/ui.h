#include <iostream>
#include "System.h"

class UI : public System {

public:

    UI() {

    }
    
    const char* GetName() const override {
        return "UI";
    }


};