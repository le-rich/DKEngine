#pragma once
#include <mutex>

#include "Entity.h"

class Scene
{
public:
    Scene();
    ~Scene();

    Entity* sceneRoot = nullptr;
    // lock to guarantee mutual exclusion
    std::mutex mtx;

    // TODO: This function loads and structures the scene graph. This should be made virtual only.
    void SpawnSceneDefinition();
    Entity* GetSceneCopy();
};