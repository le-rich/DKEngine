#pragma once

#include "Entity.h"

#include <mutex>

class Scene
{
public:
    Scene();
    ~Scene();

    Entity* getRoot() {
       return sceneRoot;
    };

    Entity* sceneRoot = nullptr;
    std::mutex sceneMutex;

    void SpawnSceneDefinition();
    Entity* GetSceneCopy();

    inline void SetSkyboxID(const UUIDv4::UUID& pSkyboxID) { mSkyboxID = pSkyboxID; }
    inline UUIDv4::UUID GetSkyboxID() { return mSkyboxID; }
private:
    std::string SCENE_FILE = "Scenes/TestScene.json";
    UUIDv4::UUID mSkyboxID;
    virtual void createGameManager();
};