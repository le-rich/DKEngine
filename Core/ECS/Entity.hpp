#include <ECS/Utils/IDUtils.hpp>
#include <ECS/Managers/ComponentManager.hpp>

template<typename T>
class Entity {

    // unique id for entity
    static const ObjectID ENTITY_ID;

    // ComponentArray attachedComponents;

    // retrieve id of entity
    static get_entity_id 
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