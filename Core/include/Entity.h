#pragma once

#include "Utils/IDUtils.h"
#include "Component.h"

#include <algorithm>
#include <vector>

//template<typename T>
class Entity {

protected:
    // unique id for entity
    UUIDv4::UUID entityID;

    // changeable display id for entity
    std::string entityDisplayName;

    // array of components
    std::vector<Component> components;


public:
    // retrieve id of entity
    UUIDv4::UUID GetEntityID() {
        return entityID;
    }

    // set the entity uuid
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

    // get array of components
    const std::vector<Component>& getComponents() const {
        return components;
    }

    // update components
    void update()
    {
        // TODO:: call the component manager's update
    }

    // add component to the entity
    void addComponent(Component& c)
    {
        components.push_back(c);
    }

    // remove component from the entity
    void removeComponent(Component& c)
    {
        components.erase(
            std::remove_if(components.begin(), components.end(),
                [&c](const Component& comp) { return comp == c; }),
            components.end());
    }

    // find a component
    Component getComponent(Component& c)
    {
        auto it = std::find_if(components.begin(), components.end(),
            [&c](const Component& comp) { return comp == c; });

        if (it != components.end()) {
            return *it;
        }
        else {
            std::cout << "Component not found" << std::endl;
        }
    }

    // default constructor
    Entity()
    {
        this->entityID = uuidGen.getUUID();
    }

    // default destructor
    ~Entity(){
        // destructor code        
    }

    // TODO: enable/disable
    
};