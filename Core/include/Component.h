#ifndef COMPONENT_H
#define COMPONENT_H

#include "Utils/IDUtils.h"

#include <string>

template<typename T>
class Component {

    // unique id for component
    UUIDv4::UUID COMPONENT_ID;

    // changeable display id for component
    std::string ComponentDisplayID;

    // retrieve id for component
    UUIDv4::UUID GetComponentID(){
        return COMPONENT_ID;
    }

    // changes the uuid for component
    void SetComponentID(UUIDv4::UUID& newID) {
        this->COMPONENT_ID = newID;
    }

    // retrieve display id of component
    std::string GetDisplayID()
    {
        return ComponentDisplayID;
    }

    // set display id of component
    void SetDisplayID(std::string newID)
    {
        ComponentDisplayID = newID;
    }

    // default constructor
    Component(){
        this->COMPONENT_ID = uuidGen.getUUID();
    }

    // destructor
    ~Component(){
        // removal code
    }
};

#endif 