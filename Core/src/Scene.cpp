#pragma once

#include <vector>

#include "Scene.h"

#include "Entities/CameraEntity.h"
#include "Entities/LightEntity.h"
//#include "GLTFLoader.h"
#include "SceneParser.h"
#include "Managers/EntityManager.h"
#include "Components/MeshComponent.h"
#include "Components/ScriptComponent.h"
#include "GarageScript.h"
#include "OrbitScript.h"
#include "TimerScript.h"
#include "LapManagerScript.h"
#include "LapCheckpointScript.h"
#include "FollowCamScript.h"
#include <CarControllerScript.h>
//#include "Input.h"

Scene::Scene()
{
    sceneRoot = new Entity("root");
    EntityManager::getInstance().addEntityToMap(*sceneRoot);
}

Scene::~Scene()
{

}

void Scene::SpawnSceneDefinition()
{
    SceneParser::LoadScene(SCENE_FILE);
    //createGameManager(); // GameManager currently setup via SceneJSON

    auto* entityManager = &(EntityManager::getInstance());

    Entity* raceTrackEnt = entityManager->findFirstEntityByDisplayName("Race Track");
    Entity* carEnt = entityManager->findFirstEntityByDisplayName("Test Car");

    // TODO: move waypoint visual creation to the lapcheckpointscripts
    Entity* cubeEntity = entityManager->findFirstEntityByDisplayName("testcube");
    cubeEntity->SetDisplayName("CheckpointModel1");
    Entity* cubeEntityCopy = entityManager->duplicateEntity(cubeEntity);
    cubeEntityCopy->SetDisplayName("CheckpointModel2");
    

    Entity* checkpoint1 = entityManager->findFirstEntityByDisplayName("Checkpoint 1");
    cubeEntity->setParent(checkpoint1);

    Entity* checkpoint2 = entityManager->findFirstEntityByDisplayName("Checkpoint 2");
    cubeEntityCopy->setParent(checkpoint2);

    // Garage Room
    Entity* garageRoomController = new Entity();
    garageRoomController->SetDisplayName("Garage Controller");
    EntityManager::getInstance().Instantiate(garageRoomController);

    // spin
    Entity* cameraEnt = entityManager->findFirstEntityByDisplayName("Main Camera");
    ScriptComponent* orb = dynamic_cast<ScriptComponent*>(cameraEnt->getComponent(ComponentType::Script));
    OrbitScript* orbScript = orb->GetScript<OrbitScript>();

    auto garageRoomEnt = entityManager->findFirstEntityByDisplayName("Garage Controller");
    ScriptComponent* garageScriptComponent = new ScriptComponent(garageRoomEnt);
    GarageScriptParams garageParams;
    garageParams.orbitScript = orbScript;
    garageParams.cars.push_back(entityManager->findFirstEntityByDisplayName("Test Car"));
    garageParams.cars.push_back(entityManager->findFirstEntityByDisplayName("TestCar2"));
    garageParams.chosenTarget = garageParams.cars[0]->transform;

    garageScriptComponent->CreateAndAddScript<GarageScript>(&garageParams);
    garageRoomEnt->addComponent(*garageScriptComponent);
    auto garageScript = garageScriptComponent->GetScript<GarageScript>();
    garageScript->BindSelectKey();
    garageScript->BindChooseKey();
}

void Scene::createGameManager()
{
    Entity* gameManager = new Entity();
    gameManager->SetDisplayName("GameManager");
    EntityManager::getInstance().Instantiate(gameManager);

    auto* entityManager = &(EntityManager::getInstance());
    auto gameManagerEnt = entityManager->findFirstEntityByDisplayName("GameManager");

    ScriptComponent* gameManagerScriptComponent = new ScriptComponent(gameManagerEnt);
    gameManagerEnt->addComponent(*gameManagerScriptComponent);

    auto carEnt = entityManager->findFirstEntityByDisplayName("Test Car");

    ScriptComponent* timerScriptComponent = new ScriptComponent(gameManagerEnt);
    gameManagerEnt->addComponent(*timerScriptComponent);

    TimerScriptParams timerParams;
    timerParams.m_TimerTarget = carEnt->transform;
    timerParams.m_OriginalPosition = carEnt->transform->getWorldPosition();
    gameManagerScriptComponent->CreateAndAddScript<TimerScript>(&timerParams);

    // Set LapManagerScript parameters & add to component
    LapManagerScriptParams lapManagerParams;

    auto checkpoint1 = entityManager->findFirstEntityByDisplayName("Checkpoint 1");
    auto checkpoint2 = entityManager->findFirstEntityByDisplayName("Checkpoint 2");
    LapCheckpointScript* checkpointScript = dynamic_cast<ScriptComponent*>(checkpoint1->getComponent(ComponentType::Script))->GetScript<LapCheckpointScript>();
    LapCheckpointScript* checkpointScript2 = dynamic_cast<ScriptComponent*>(checkpoint2->getComponent(ComponentType::Script))->GetScript<LapCheckpointScript>();

    std::vector<LapCheckpointScript*> checkpoints = { checkpointScript, checkpointScript2 };
    lapManagerParams.m_Checkpoints = checkpoints;
    gameManagerScriptComponent->CreateAndAddScript<LapManagerScript>(&lapManagerParams);

    // Set LapManager vars of checkpoints
    LapManagerScript* lapManagerPointerFromScript = gameManagerScriptComponent->GetScript<LapManagerScript>();
    checkpointScript->SetLapManager(lapManagerPointerFromScript);
    checkpointScript2->SetLapManager(lapManagerPointerFromScript);

}

Entity* Scene::GetSceneCopy()
{
    Entity* cur = new Entity(*sceneRoot);
    return cur;
}
