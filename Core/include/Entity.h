#pragma once

#include "Utils/IDUtils.h"
#include "Component.h"
#include "Components/TransformComponent.h"

#include <algorithm>
#include <memory>
#include <vector>

//template<typename T>
class Entity
{

protected:
    // unique id for entity
    UUIDv4::UUID entityID;

    // changeable display id for entity
    std::string entityDisplayName;

    // array of components
    std::vector<Component*> components;
    ComponentMask componentMask;

    // pointer to its parent Entity
    Entity* parent = nullptr;

    // list of child entities
    std::vector<Entity*> children;

public:
    // Every entity should by default have a transform.
    TransformComponent* transform;

    // retrieve id of entity
    UUIDv4::UUID GetEntityID()
    {
        return entityID;
    }

    ComponentMask GetComponentMask() const { 
        return componentMask;
    }

    // set the entity uuid
    void SetEntityID(const UUIDv4::UUID& newid)
    {
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
    const std::vector<Component*>& getComponents() const
    {
        return components;
    }


    // update components
    void update()
    {
        // TODO:: Entities themselves should not have updates, but their components should.
    }

    // add component to the entity
    void addComponent(Component& c)
    {
        components.push_back(&c);

        componentMask.set(static_cast<size_t>(c.componentType));
    }

    // remove component from the entity
    void removeComponent(Component& c)
    {
        components.erase(
            std::remove_if(components.begin(), components.end(),
                [&c](const Component* comp) { return comp == &c; }),
            components.end());

        componentMask.reset(static_cast<size_t>(c.componentType));
    }

    Component* getComponent(const ComponentType& componentType)
    {
        auto it = std::find_if(components.begin(), components.end(),
            [&componentType](const Component* comp) { return comp->componentType == componentType; });

        if (it != components.end())
        {
            return *it;
        }
        else
        {
            return nullptr;
        }
    }

    bool HasComponent(ComponentType type) const {
        return componentMask.test(static_cast<size_t>(type));
    }

    // default constructor
    Entity()
    {
        this->entityID = uuidGen.getUUID();
        this->transform = new TransformComponent(this);
        // init code
    }

    Entity(std::string DisplayName)
    {
        this->entityDisplayName = DisplayName;
        this->entityID = uuidGen.getUUID();
        this->transform = new TransformComponent(this);
    }

    // Copy constructor
    Entity(const Entity& other);

    // default destructor
    ~Entity()
    {
        // destructor code        
    }

    // parent code

    // set parent entity
    void setParent(Entity* parentEntity)
    {
        if (this->parent != nullptr)
        {
            this->parent->removeChild(this);
        }
        this->parent = parentEntity;

        if (parentEntity != nullptr)
        {
            parentEntity->addChild(this);
        }
        // TODO: After updating transform matrix, locals should be updated to reflect positioning relative to parent.
        // ie. Update locals such that the object doesn't change world position
        // Execption: Model loading
        // The transforms in the models are already relative to the parent
        // As such this should only effect reparenting of entities AFTER they've been loaded into the scene.
    }

    // retrieve parent entity
    Entity* getParent() const
    {
        return parent;
    }

    // add a child entity
    void addChild(Entity* child)
    {
       for (Entity* e : children) {
          if (e == child) {
             return;
          }
       }
        children.push_back(child);
        child->parent = this;
    }

    // remove a child entity
    void removeChild(Entity* child)
    {
        children.erase(
            std::remove_if(children.begin(), children.end(),
                [child](Entity* c) { return c == child; }),
            children.end());
    }

    // remove all children entities
    void clearChildren() {
        children.clear();
    }

    // retrieve all children entities
    const std::vector<Entity*>& getChildren() const
    {
        return children;
    }

    // retrieve a specific component from a parent
    Component* getComponentFromParent(const ComponentType& componentType)
    {
        Component* comp = getComponent(componentType);
        if (comp != nullptr)
        {
            return comp;
        }
        if (parent != nullptr)
        {
            return parent->getComponentFromParent(componentType);
        }
        return nullptr;
    }

    bool operator==(const Entity& other) const {
        return (other.entityID == this->entityID &&
            other.entityDisplayName == this->entityDisplayName &&
            other.getComponents() == this->getComponents());
    }

    // TODO: enable/disable

};