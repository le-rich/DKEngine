#pragma once

#include "../include/Entity.h"
#include "Utils/IDUtils.h"

#include <queue>

class EntityManager{

protected:
    static const int MAX_ENTITIES = 3000;
    // TODO: map of entities

public:
    // default constructor
    EntityManager(){
        // iterate over map
        std::cout << "Entity Manager created" << std::endl;
    }

    // TODO: method to remove entities from map
    // TODO: method to add entities to map
    // TODO: method to retrieve entities from map
    // TODO: complete destructor

};