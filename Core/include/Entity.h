#pragma once

#include "Utils/IDUtils.h"
#include "Managers/ComponentManager.h"

//template<typename T>
class Entity {

protected:
    // unique id for entity
    UUIDv4::UUID ENTITY_ID;

    // changeable display id for entity
    std::string EntityDisplayID;

    // attached component manager
    ComponentManager* componentManager;


public:
    // retrieve id of entity
    UUIDv4::UUID GetEntityID() {
        return ENTITY_ID;
    }

    void SetEntityID(const UUIDv4::UUID& newid) {
        this->ENTITY_ID = newid;
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
    Entity()
    {
        this->ENTITY_ID = uuidGen.getUUID();
        // init code
    }

    // overloaded constructor
    Entity(ComponentManager* compMngr)
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