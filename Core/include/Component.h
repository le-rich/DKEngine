#pragma once

#include "Utils/IDUtils.h"

#include <bitset>
#include <string>

class Entity;
class Transform;

// An Enum registry of Components. Annoying to have to keep track, but necessary for querying the entity tree.
enum class ComponentType : uint8_t {
    None = 0,
    Transform, 
    Camera,
    Light,
    Mesh,
    Script,
    Count, // This needs to be last. It represents how many components there are for bitmask.
};

// Defines a type - Component Mask - that is a bitmask of components on an entity. Used for querying.
using ComponentMask = std::bitset<static_cast<size_t>(ComponentType::Count)>;

class Component
{

public:
    Component(Entity* mEntity);
    virtual ~Component() = default;

    Entity* entity;

    UUIDv4::UUID componentID;
    std::string componentDisplayName;
    ComponentType componentType;

    UUIDv4::UUID GetComponentID()
    {
        return componentID;
    }

    void SetComponentID(UUIDv4::UUID& newID)
    {
        this->componentID = newID;
    }

    const std::string& GetDisplayName()
    {
        return componentDisplayName;
    }

    // set display id of component
    void SetDisplayName(std::string newID)
    {
        componentDisplayName = newID;
    }

    bool operator==(const Component& other) const;

private:
    
};
