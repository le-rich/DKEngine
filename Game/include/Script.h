#pragma once

#include "Entity.h"

class Script {
public:
    Script(Entity* mEntity) : entity(mEntity) {};
    virtual ~Script() = default;
    virtual void Update(float deltaTime) = 0;

    Entity* entity = nullptr;
};
