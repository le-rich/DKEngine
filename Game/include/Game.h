#pragma once
#include "System.h"

class Game : public System 
{
public:
    Game();
    ~Game();

    void Initialize() override;
    void Update(float deltaTime) override;
    void FixedUpdate() override;

    const char* GetName() const override {
        return "Game";
    }

private:

};
