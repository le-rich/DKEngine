#pragma once

#include "Core.h"
#include "../include/Entity.h"
#include "Scene.h"

#include <Utils/IDUtils.h>

#include <map>

class EntityManager
{

protected:
    static const int MAX_ENTITIES = 3000;
    std::map<UUIDv4::UUID, Entity*> entityMap;

    // Private constructor as this should be a singleton.
    EntityManager()
    {

    }

public:
    // Delete copy constructor and copy assignment operator
    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;

    // default constructor
    static EntityManager& getInstance()
    {
        static EntityManager instance;
        return instance;
    }

    // TODO: Remove this function, should be done elsewhere
    // iterates over all entities in the map and calls the entity's update function
    void activate()
    {
        for (auto& pair : entityMap)
        {
            pair.second->update();
        }
    }

    // removes an entity from the map by UUID
    void removeEntity(UUIDv4::UUID& eID)
    {
        auto it = entityMap.find(eID);
        if (it != entityMap.end())
        {
           removeEntity(*(it->second));
        }
    }

    // removes an entity from the map by Entity reference
    void removeEntity(Entity& e) {
        UUIDv4::UUID eID = e.GetEntityID();
        auto it = entityMap.find(eID);

        if (it != entityMap.end()) {
           while (it->second->getChildren().size() != 0) {
              Entity* child = it->second->getChildren()[0];
              removeEntity(*child);
              child->getParent()->removeChild(child);
           }
           it->second->getParent()->removeChild(it->second);
            entityMap.erase(it);
        }
        else {
            std::cout << "Entity not found in the map." << std::endl;
        }
    }

    // add an entity to the map
    void addEntityToMap(Entity& e)
    {
        for (Entity* childEntity : e.getChildren())
        {
            addEntityToMap(*childEntity);
        }

        auto result = entityMap.insert({ e.GetEntityID(), &e });

        if (result.second)
        {
            // std::cout << "Entity added" << std::endl;
        }
        else {
            // check if the entity with the same UUID is actually the same entity
            Entity* existingEntity = result.first->second;
            if (*existingEntity == e) {
                std::cout << "Entity already exists in the map." << std::endl;
            }
            else {
                // regenerates UUID for the new entity
                UUIDv4::UUID newID = uuidGen.getUUID();
                e.SetEntityID(newID);
                addEntityToMap(e);
            }
        }
    }

    // retrieve an entity from the map
    Entity* getEntity(UUIDv4::UUID& eID)
    {
        auto it = entityMap.find(eID);

        if (it != entityMap.end())
        {
            return it->second;
        }
        else {
            std::cerr << "Error: Entity with UUID " << eID << " not found in the map." << std::endl;
            return nullptr;
        }
    }

    std::vector<UUIDv4::UUID> findEntitiesByComponent(const ComponentType componentType)
    {
        std::vector<UUIDv4::UUID> result;

        for (auto it = entityMap.begin(); it != entityMap.end(); ++it)
        {
            const UUIDv4::UUID& uuid = it->first;
            const Entity& entity = *it->second;

            if (entity.HasComponent(componentType)) {
                result.push_back(it->first);
            }
        }

        return result;
    }

    // retrieve by display name
    Entity* findFirstEntityByDisplayName(const std::string& displayName)
    {
        for (auto it = entityMap.begin(); it != entityMap.end(); ++it)
        {
            UUIDv4::UUID uuid = it->first;
            Entity* entity = it->second;

            if (entity->GetDisplayName() == displayName)
            {
                return it->second;
            }
        }

        return nullptr;
    }

    // Returns entities explicitly matching the component mask
    std::vector<Entity*> findEntitiesByComponentMask(ComponentMask componentMask) 
    {
        std::vector<Entity*> result;
        for (auto it = entityMap.begin(); it != entityMap.end(); ++it)
        {
            UUIDv4::UUID uuid = it->first;
            Entity* entity = it->second;

            if (entity->GetComponentMask() == componentMask)
            {
                result.push_back(entity);
            }
        }

        return result;
    }

    // Returns entities that contain the component mask
    std::vector<Entity*> findEntitiesContainingComponentMask(ComponentMask componentMask)
    {
        std::vector<Entity*> result;
        for (auto it = entityMap.begin(); it != entityMap.end(); ++it)
        {
            UUIDv4::UUID uuid = it->first;
            Entity* entity = it->second;

            if ((entity->GetComponentMask() & componentMask) == componentMask)
            {
                result.push_back(entity);
            }
        }

        return result;
    }

    // clean and remove entity from the tree
    void cleanAndRemoveEntity(Entity* e) {
        // if a parent exists, remove the reference of the entity from it
        Entity* parent = e->getParent();
        if (parent) {
            parent->removeChild(e);
        }

        // recursively yeet children
        for (auto& child : e->getChildren()) {
            cleanAndRemoveEntity(child);
        }
        e->clearChildren();

        // remove it from the map
        removeEntity(*e);
    }

    void Instantiate(Entity* entity)
    {
        Core::getInstance().GetScene()->sceneRoot->addChild(entity);
        entity->setParent(Core::getInstance().GetScene()->sceneRoot);

        addEntityToMap(*entity);
    }

    void Instantiate(Entity* entity, Entity* parent)
    {
        Scene* scene = Core::getInstance().GetScene();
        UUIDv4::UUID parentUUID = parent->GetEntityID();

        getEntity(parentUUID)->addChild(entity);
        entity->setParent(parent);
    }

    void Instantiate(Entity* entity, TransformComponent* transform) {
        Core::getInstance().GetScene()->sceneRoot->addChild(entity);
        entity->setParent(Core::getInstance().GetScene()->sceneRoot);

        entity->transform = transform;

        addEntityToMap(*entity);
    }

    void Instantiate(Entity* entity, TransformComponent* transform, Entity* parent)
    {
        Scene* scene = Core::getInstance().GetScene();
        UUIDv4::UUID parentUUID = parent->GetEntityID();

        getEntity(parentUUID)->addChild(entity);
        entity->setParent(parent);

        entity->transform = transform;

        addEntityToMap(*entity);
    }

    Entity* duplicateEntity(Entity* originalEntity) {
        if (!originalEntity) return nullptr;

        // create a new entity with a copy name
        Entity* duplicate = new Entity(originalEntity->GetDisplayName() + "_copy");

        duplicate->transform = dynamic_cast<TransformComponent*>(originalEntity->transform->clone());
        duplicate->transform->entity = duplicate;

        // duplicate components
        for (const auto& comp : originalEntity->getComponents()) {
            auto newComp = comp->clone();
            newComp->entity = duplicate;
            duplicate->addComponent(*newComp);
        }

        // recursive duplicate children
        for(int i = 0; i < originalEntity->getChildren().size(); i++){
            Entity* childDuplicate = duplicateEntity(originalEntity->getChildren()[i]);
            childDuplicate->setParent(duplicate);
            duplicate->addChild(childDuplicate);
        }

        duplicate->setParent(originalEntity->getParent());

        addEntityToMap(*duplicate);

        return duplicate;
    }
};