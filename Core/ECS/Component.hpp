#include <ECS/Utils/IDUtils.hpp>

template<typename T>
class Component {

    // unique id for component
    static const ObjectID COMPONENT_ID;

    // retrieve id for component
    ObjectID GetComponentID() {
        return COMPONENT_ID;
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