#pragma once

#include "Utils/IDUtils.h"
#include "Managers/ComponentManager.h"

template<typename T>
class Entity {

    // unique id for entity
    static const ObjectID ENTITY_ID;

    // retrieve id of entity
    static ObjectID GetEntityId()
    {
        return ENTITY_ID;
    }

    // default constructor
    Entity(const ObjectID signature, ComponentManager* compMngr)
    {
        // init code
    }

    ~Entity(){
        // destructor code        
    }

    // TODO: method to retrieve individual component
    // TODO: method to add component
    // TODO: method to destroy component
    // TODO: enable/disable
    
};