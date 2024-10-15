#pragma once

#include "Core.h"
#include "../include/Entity.h"
#include "Utils/IDUtils.h"

#include <map>

class EntityManager{

protected:
    static const int MAX_ENTITIES = 3000;
    // TODO: map of entities
    std::map<UUIDv4::UUID, Entity*> entityMap;

    // Private constructor as this should be a singleton.
    EntityManager(){
        std::cout << "Entity Manager created" << std::endl;
    }

public:
    // Delete copy constructor and copy assignment operator
    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;

    // default constructor
    static EntityManager& getInstance() {
        static EntityManager instance;
        return instance;
    }

    // TODO: Remove this function, should be done elsewhere
    // iterates over all entities in the map and calls the entity's update function
    void activate() {
        for (auto& pair : entityMap) {
            pair.second->update();
        }
    }

    // removes an entity from the map by UUID
    void removeEntity(UUIDv4::UUID& eID) {
        auto it = entityMap.find(eID);
        if (it != entityMap.end()) {
            entityMap.erase(it);
        }
    }

    // add an entity to the map
    void addEntityToMap(Entity& e) {
        auto result = entityMap.insert({ e.GetEntityID(), &e });

        if (result.second) {
            std::cout << "Entity added" << std::endl;
        }
        else {
            // regenerates UUID for the new entity
            // TODO at some point: add a guard to check if its just the exact same entity being added in, need some time to thonk on how
            UUIDv4::UUID newID = uuidGen.getUUID();
            e.SetEntityID(newID);
            addEntityToMap(e);
        }
    }

    // retrieve an entity from the map
    Entity* getEntity(UUIDv4::UUID& eID) {
        auto it = entityMap.find(eID);

        if (it != entityMap.end()) {
            return it->second;
        }
        else {
            // TODO : handle if the entity isn't in the map?
            return nullptr;
        }
    }

    // queries the entity map to return an array of entities with the component
    std::vector<UUIDv4::UUID> findEntitiesByComponent(const Component& component) {
        std::vector<UUIDv4::UUID> result;

        for (auto it = entityMap.begin(); it != entityMap.end(); ++it) {
            const UUIDv4::UUID& uuid = it->first;
            const Entity& entity = *it->second;

            if (std::find(entity.getComponents().begin(), entity.getComponents().end(), component) != entity.getComponents().end()) {
                result.push_back(uuid);
            }
        }

        return result;
    }

    // retrieve by display name
    UUIDv4::UUID findFirstEntityByDisplayName(const std::string& displayName) {
        for (auto it = entityMap.begin(); it != entityMap.end(); ++it) {
            UUIDv4::UUID uuid = it->first;
            Entity entity = *it->second;

            if (entity.GetDisplayName() == displayName) {
                return uuid;
            }
            else {
                std::cout << "Entity not found." << std::endl;
                // TODO: This should return a path. It will lead to unexpected behaviour if it doesn't.
            }
        }
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

    void Instantiate(Entity* entity, Transform* transform) {
        Core::getInstance().GetScene()->sceneRoot->addChild(entity);
        entity->setParent(Core::getInstance().GetScene()->sceneRoot);

        entity->transform = transform;

        addEntityToMap(*entity);
    }

    void Instantiate(Entity* entity, Transform* transform, Entity* parent)
    {
        Scene* scene = Core::getInstance().GetScene();
        UUIDv4::UUID parentUUID = parent->GetEntityID();

        getEntity(parentUUID)->addChild(entity);
        entity->setParent(parent);

        entity->transform = transform;

        addEntityToMap(*entity);
    }
};