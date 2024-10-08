#pragma once

#include "../include/Entity.h"
#include "Utils/IDUtils.h"

#include <map>

class EntityManager{

protected:
    static const int MAX_ENTITIES = 3000;
    // TODO: map of entities
    std::map<UUIDv4::UUID, Entity> entityMap;

public:
    // default constructor
    EntityManager(){
        std::cout << "Entity Manager created" << std::endl;
    }

    // iterates over all entities in the map and calls the entity's update function
    void activate() {
        for (auto& pair : entityMap) {
            pair.second.update();
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
    void addEntity(Entity& e) {
        auto result = entityMap.insert({ e.GetEntityID(), e });

        if (result.second) {
            std::cout << "Entity added" << std::endl;
        }
        else {
            // regenerates UUID for the new entity
            // TODO at some point: add a guard to check if its just the exact same entity being added in, need some time to thonk on how
            UUIDv4::UUID newID = uuidGen.getUUID();
            e.SetEntityID(newID);
            addEntity(e);
        }
    }

    // retrieve an entity from the map
    Entity* getEntity(UUIDv4::UUID& eID) {
        auto it = entityMap.find(eID);

        if (it != entityMap.end()) {
            return &it->second;
        }
        else {
            // TODO : handle if the entity isn't in the map?
        }
    }

    // queries the entity map to return an array of entities with the component
    std::vector<UUIDv4::UUID> findEntitiesByComponent(const Component& component) {
        std::vector<UUIDv4::UUID> result;

        for (auto it = entityMap.begin(); it != entityMap.end(); ++it) {
            const UUIDv4::UUID& uuid = it->first;
            const Entity& entity = it->second;

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
            Entity entity = it->second;

            if (entity.GetDisplayName() == displayName) {
                return uuid;
            }
            else {
                std::cout << "Entity not found." << std::endl;
            }
        }

    }
};