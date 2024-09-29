#ifndef COMPONENT_H
#define COMPONENT_H

#include "Utils/IDUtils.h"

#include <string>

template<typename T>
class Component {

    // unique id for component
    UUIDv4::UUID componentID;

    // changeable display id for component
    std::string displayName;

    // retrieve id for component
    UUIDv4::UUID GetComponentID(){
        return componentID;
    }

    // changes the uuid for component
    void SetComponentID(UUIDv4::UUID& newID) {
        this->componentID = newID;
    }

    // retrieve display id of component
    std::string GetDisplayID()
    {
        return displayName;
    }

    // set display id of component
    void SetDisplayID(std::string newID)
    {
        displayName = newID;
    }

    // default constructor
    Component(){
        this->componentID = uuidGen.getUUID();
    }

    // destructor
    ~Component(){
        // removal code
    }
};

#endif 