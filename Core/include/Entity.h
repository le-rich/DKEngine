#pragma once

#include "Utils/IDUtils.h"
#include "Managers/ComponentManager.h"

//template<typename T>
class Entity {

protected:
    // unique id for entity
    UUIDv4::UUID entityID;

    // changeable display id for entity
    std::string entityDisplayName;

    // attached component manager
    ComponentManager* compMngr;


public:
    // retrieve id of entity
    UUIDv4::UUID GetEntityID() {
        return entityID;
    }

    void SetEntityID(const UUIDv4::UUID& newid) {
        this->entityID = newid;
    }

    // retrieve display id of entity
    std::string GetDisplayName()
    {
        return entityDisplayName;
    }

    // set display id of entity
    void SetDisplayName(std::string newID)
    {
        entityDisplayName = newID;
    }

    void update()
    {
        // TODO:: call the component manager's update
    }

    // default constructor
    Entity()
    {
        this->entityID = uuidGen.getUUID();
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