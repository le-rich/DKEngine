#include "Utils/IDUtils.h"

#include <string>

template<typename T>
class Component {

    // unique id for component
    ObjectID COMPONENT_ID;

    // changeable display id for component
    std::string ComponentDisplayID;

    // retrieve id for component
    ObjectID GetComponentID(){
        return COMPONENT_ID;
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
        // init code
    }

    // destructor
    ~Component(){
        // removal code
    }
};