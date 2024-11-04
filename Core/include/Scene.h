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
    
    void setRoot(Entity* e) {
       sceneRoot = e;
    };

    Entity* sceneRoot = nullptr;

    // TODO: This function loads and structures the scene graph. This should be made virtual only.
    void SpawnSceneDefinition();
};