#pragma once

#include "Utils/IDUtils.h"
#include "Component.h"
#include "Components/Transform.h"

#include <algorithm>
#include <memory>
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

    // pointer to its parent Entity
    Entity* parent = nullptr;

    // list of child entities
    std::vector<Entity*> children;

public:
    // Every entity should by default have a transform.
    Transform* transform;

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
                [&c](const Component& comp) { return &comp == &c; }),
            components.end());
    }
    
    Component* getComponent(const Component& c) {
        auto it = std::find_if(components.begin(), components.end(),
            [&c](const Component& comp) { return &comp == &c; });

        if (it != components.end()) {
            return &(*it);
        }
        else {
            return nullptr;
        }
    }

    // default constructor
    Entity()
    {
        this->entityID = uuidGen.getUUID();
        this->transform = new Transform(this);
        // init code
    }

    // default destructor
    ~Entity(){
        // destructor code        
    }

    // parent code

    // set parent entity
    void setParent(Entity* parentEntity) {
        if (this->parent != nullptr) {
            this->parent->removeChild(this);
        }
        this->parent = parentEntity;

        if (parentEntity != nullptr) {
            parentEntity->addChild(this);
        }
    }

    // retrieve parent entity
    Entity* getParent() const {
        return parent;
    }

    // add a child entity
    void addChild(Entity* child) {
        children.push_back(child);
        child->parent = this;
    }

    // remove a child entity
    void removeChild(Entity* child) {
        children.erase(
            std::remove_if(children.begin(), children.end(),
                [child](Entity* c) { return c == child; }),
            children.end());
    }

    // retrieve all children entities
    const std::vector<Entity*>& getChildren() const {
        return children;
    }

    // retrieve a specific component from a parent
    Component* getComponentFromParent(const Component& c) {
        Component* comp = getComponent(c);
        if (comp != nullptr) {
            return comp;
        }
        if (parent != nullptr) {
            return parent->getComponentFromParent(c);
        }
        return nullptr;
    }

    bool operator==(const Entity& other) const {
        return (other.entityID == this->entityID);
    }

    // TODO: enable/disable
    
};