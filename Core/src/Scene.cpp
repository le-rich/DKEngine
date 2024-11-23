#pragma once

#include <vector>

#include "Scene.h"

#include "Entities/CameraEntity.h"
#include "Entities/LightEntity.h"
#include "GLTFLoader.h"
#include "Managers/EntityManager.h"
#include "Components/MeshComponent.h"
#include "Components/ScriptComponent.h"
#include "GarageScript.h"
#include "OrbitScript.h"
#include "TimerScript.h"
#include "LapManagerScript.h"
#include "LapCheckpointScript.h"
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
    CameraEntity* cameraEntity = new CameraEntity();
    cameraEntity->SetDisplayName("Main Camera");
    cameraEntity->transform->setWorldPosition(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    EntityManager::getInstance().Instantiate(cameraEntity);

    LightEntity* lightEntity = new LightEntity();
    lightEntity->SetDisplayName("Light");
    lightEntity->transform->setLocalPosition(glm::vec3(0.f, 5.f, 0.f));
    lightEntity->transform->setLocalOrientation(glm::quat(-0.5f, 0.5f, 0.5f, 0.f));
    EntityManager::getInstance().Instantiate(lightEntity);

    //Input& input = Input::GetInstance();

    // TODO: JSONparser for Scene entities and scripts/components.
    // TODO: Figure out location and pathing of assets/non code files within solution
    const std::string SOURCE_FOLDER = "TestAE2/";
    const std::string MODEL_FILE = "ae86.gltf";
    const std::string CAR_2_SOURCE_FOLDER = "Car2Test/";
    const std::string CAR_2_MODEL_FILE = "sportcar.017.gltf";

    Entity* testCar = new Entity();
    testCar->SetDisplayName("Test Car");
    GLTFLoader::LoadModelAsEntity(testCar, SOURCE_FOLDER, MODEL_FILE);
    EntityManager::getInstance().Instantiate(testCar);

    // making a 2nd car
    Entity* testCar2 = new Entity();
    testCar2->SetDisplayName("Test Car 2");
    GLTFLoader::LoadModelAsEntity(testCar2, CAR_2_SOURCE_FOLDER, CAR_2_MODEL_FILE);
    EntityManager::getInstance().Instantiate(testCar2);

    Entity* gameManager = new Entity();
    gameManager->SetDisplayName("Game Manager");
    EntityManager::getInstance().Instantiate(gameManager);

    // Example of using findFirstEntityByDisplayName() for adding additional components
    auto* entityManager = &(EntityManager::getInstance());
    auto cameraID = entityManager->findFirstEntityByDisplayName("Main Camera");
    auto cameraEnt = entityManager->getEntity(cameraID);

    ScriptComponent* cameraScriptComponent = new ScriptComponent(cameraEnt);
    cameraEnt->addComponent(*cameraScriptComponent);

    auto carID = entityManager->findFirstEntityByDisplayName("Test Car");
    auto carEnt = entityManager->getEntity(carID);

    // making a 2nd car
    auto car2ID = entityManager->findFirstEntityByDisplayName("Body");
    auto carEnt2 = entityManager->getEntity(car2ID);
    carEnt2->transform->setLocalPosition(glm::vec3(1.5f, 0.0f, 0.0f));
    carEnt2->transform->setLocalScale(glm::vec3(0.4f, 0.4f, 0.4f));
    

    OrbitScriptParams orbitParams;
    orbitParams.m_OrbitTarget = carEnt->transform;
    orbitParams.m_OrbitTarget2 = carEnt2->transform;
    cameraScriptComponent->AddScriptToComponent<OrbitScript>(&orbitParams);
    OrbitScript* orbitScript = cameraScriptComponent->GetScript<OrbitScript>();

    // Garage Room
    Entity* garageRoomController = new Entity();
    garageRoomController->SetDisplayName("Garage Controller");
    EntityManager::getInstance().Instantiate(garageRoomController);

    auto garageRoomID = entityManager->findFirstEntityByDisplayName("Garage Controller");
    auto garageRoomEnt = entityManager->getEntity(garageRoomID);
    ScriptComponent* garageScriptComponent = new ScriptComponent(garageRoomEnt);
    garageRoomEnt->addComponent(*garageScriptComponent);

    /*
    input.RegisterKeyCallback(GLFW_KEY_SPACE, [orbitScript](Input::ActionType action) {
        if (action == Input::PRESS) {
            orbitScript->swapOrbitTargets();
        }
    });
    */
    // End Example

    auto gameManagerID = entityManager->findFirstEntityByDisplayName("Game Manager");
    auto gameManagerEnt = entityManager->getEntity(gameManagerID);

    ScriptComponent* timerScriptComponent = new ScriptComponent(gameManagerEnt);
    gameManagerEnt->addComponent(*timerScriptComponent);

    TimerScriptParams timerParams;
    timerParams.m_TimerTarget = carEnt->transform;
    timerParams.m_OriginalPosition = carEnt->transform->getWorldPosition();
    timerScriptComponent->AddScriptToComponent<TimerScript>(&timerParams);

    // Create Lap Manager Entity
    Entity* lapManagerEntity = new Entity();
    lapManagerEntity->SetDisplayName("Lap Manager");
    ScriptComponent* lapManagerComponent = new ScriptComponent(lapManagerEntity);
    LapManagerScript* lapManagerScript = new LapManagerScript(lapManagerEntity);

    // Create 1st checkpoint
    Entity* checkpointEntity1 = new Entity();
    checkpointEntity1->SetDisplayName("Lap Checkpoint 1");
    checkpointEntity1->transform->setWorldPosition(glm::vec4(0.f, 0.f, 0.f, 1.0f));
    ScriptComponent* checkpointComponent1 = new ScriptComponent(checkpointEntity1);
    // Set LapCheckpointScript parameters & add to component
    LapCheckpointScriptParams checkpointParams1;
    checkpointParams1.m_Other = carEnt->transform;
    checkpointParams1.m_Index = 1;
    checkpointParams1.m_LapManager = nullptr; // might as well be nullptr
    checkpointComponent1->AddScriptToComponent<LapCheckpointScript>(&checkpointParams1);
    checkpointEntity1->addComponent(*checkpointComponent1);

    // Create 2nd checkpoint
    Entity* checkpointEntity2 = new Entity();
    checkpointEntity2->SetDisplayName("Checkpoint 2");
    checkpointEntity2->transform->setWorldPosition(glm::vec4(-1.25f, 0.f, 0.f, 1.0f));
    ScriptComponent* checkpointComponent2 = new ScriptComponent(checkpointEntity2);
    // Set LapCheckpointScript parameters & add to component
    LapCheckpointScriptParams checkpointParams2;
    checkpointParams2.m_Other = carEnt->transform;
    checkpointParams2.m_Index = 2;
    checkpointParams2.m_LapManager = nullptr; // might as well be nullptr
    checkpointComponent2->AddScriptToComponent<LapCheckpointScript>(&checkpointParams2);
    checkpointEntity2->addComponent(*checkpointComponent2);
    
    // Set LapManagerScript parameters & add to component
    LapCheckpointScript* checkpointPointerFromScript = checkpointComponent1->GetScript<LapCheckpointScript>();
    LapCheckpointScript* checkpointPointerFromScript2 = checkpointComponent2->GetScript<LapCheckpointScript>();
    LapManagerScriptParams lapManagerParams;
    std::vector<LapCheckpointScript*> checkpoints = { checkpointPointerFromScript, checkpointPointerFromScript2 };
    lapManagerParams.m_Checkpoints = checkpoints;
    lapManagerComponent->AddScriptToComponent<LapManagerScript>(&lapManagerParams);
    lapManagerEntity->addComponent(*lapManagerComponent);
    // Set LapManager vars of checkpoints
    LapManagerScript* lapManagerPointerFromScript = lapManagerComponent->GetScript<LapManagerScript>();
    checkpointPointerFromScript->SetLapManager(lapManagerPointerFromScript);
    checkpointPointerFromScript2->SetLapManager(lapManagerPointerFromScript);

    // Instantiate entities for lap system
    EntityManager::getInstance().Instantiate(checkpointEntity1);
    EntityManager::getInstance().Instantiate(checkpointEntity2);
    EntityManager::getInstance().Instantiate(lapManagerEntity);
}

Entity* Scene::GetSceneCopy()
{
    Entity* cur = new Entity(*sceneRoot);
    return cur;
}
