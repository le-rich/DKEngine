#ifndef COMPONENT_H
#define COMPONENT_H

#include "Utils/IDUtils.h"

#include <string>

//template<typename T>
class Component {

public: 
    // unique id for component
    UUIDv4::UUID componentID;

    // changeable display id for component
    std::string componentDisplayName;

    // retrieve id for component
    UUIDv4::UUID GetComponentID(){
        return componentID;
    }

    // changes the uuid for component
    void SetComponentID(UUIDv4::UUID& newID) {
        this->componentID = newID;
    }

    // retrieve display id of component
    std::string GetDisplayName()
    {
        return componentDisplayName;
    }

    // set display id of component
    void SetDisplayName(std::string newID)
    {
        componentDisplayName = newID;
    }

    // default constructor
    Component(){
        this->componentID = uuidGen.getUUID();
    }

    // destructor
    ~Component(){
        // removal code
    }

    // overloaded equality operator based on UUID
    bool operator==(const Component& other) const {
        return componentID == other.componentID; 
    }
};

#endif 