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

    Entity* sceneRoot = nullptr;

    // TODO: This function loads and structures the scene graph. This should be made virtual only.
    void SpawnSceneDefinition();
private:
    std::string SCENE_FILE = "Scenes/TestScene.json";
    virtual void createGameManager();
};