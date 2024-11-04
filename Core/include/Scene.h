#pragma once
#include "Entity.h"


class Scene
{
public:
    Scene();
    ~Scene();

    Entity* getRoot() {
       return sceneRoot;
    };
    
    void setRoot(Entity* newRoot) {
       sceneRoot = newRoot;
    };

    Entity* sceneRoot = nullptr;

    // TODO: This function loads and structures the scene graph. This should be made virtual only.
    void SpawnSceneDefinition();
};