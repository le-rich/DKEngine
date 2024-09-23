#pragma once

#include "Utils/IDUtils.h"
#include "Managers/ComponentManager.h"

template<typename T>
class Entity {

    // unique id for entity
    ObjectID ENTITY_ID;

    // changeable display id for entity
    std::string EntityDisplayID;

    // attached component manager
    ComponentManager* compMngr;


    // retrieve id of entity
    ObjectID GetEntityId() {
        return ENTITY_ID;
    }

    // retrieve display id of entity
    std::string GetDisplayID()
    {
        return EntityDisplayID;
    }

    // set display id of entity
    void SetDisplayID(std::string newID)
    {
        EntityDisplayID = newID;
    }

    // default constructor
    Entity(ObjectID signature, ComponentManager* compMngr)
    {
        // init code
    }

    // default destructor
    ~Entity(){
        // destructor code        
    }

    // TODO: method to retrieve individual component
    // TODO: method to add component
    // TODO: method to destroy component
    // TODO: enable/disable
    
};