#pragma once

#include "Entity.h"

class Script {
public:
    Script(Entity* mEntity) : entity(mEntity) {};
    virtual ~Script() = default;
    virtual void Update(float deltaTime) = 0;

    Entity* entity = nullptr;

    virtual std::unique_ptr<Script> clone() const = 0;
};
