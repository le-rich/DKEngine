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
    /*
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

    Entity* testCar = new Entity();
    testCar->SetDisplayName("Test Car");
    GLTFLoader::LoadModelAsEntity(testCar, SOURCE_FOLDER, MODEL_FILE);
    EntityManager::getInstance().Instantiate(testCar);
    */

    // making a 2nd car
    /*
    const std::string CAR_2_SOURCE_FOLDER = "Car2Test/";
    const std::string CAR_2_MODEL_FILE = "sportcar.017.gltf";
    Entity* testCar2 = new Entity();
    testCar2->SetDisplayName("Test Car 2");
    GLTFLoader::LoadModelAsEntity(testCar2, CAR_2_SOURCE_FOLDER, CAR_2_MODEL_FILE);
    EntityManager::getInstance().Instantiate(testCar2);
    */

    auto* entityManager = &(EntityManager::getInstance());

    // Garage Room
    Entity* garageRoomController = new Entity();
    garageRoomController->SetDisplayName("Garage Controller");
    EntityManager::getInstance().Instantiate(garageRoomController);

    // spin
    auto cameraEnt = entityManager->findFirstEntityByDisplayName("Main Camera");
    auto orb = dynamic_cast<ScriptComponent*>(cameraEnt->getComponent(ComponentType::Script));
    auto orbScript = orb->GetScript<OrbitScript>();

    auto garageRoomEnt = entityManager->findFirstEntityByDisplayName("Garage Controller");
    ScriptComponent* garageScriptComponent = new ScriptComponent(garageRoomEnt);
    GarageScriptParams garageParams;
    garageParams.orbitScript = orbScript;
    garageParams.cars.push_back(entityManager->findFirstEntityByDisplayName("Test Car"));
    garageParams.cars.push_back(entityManager->findFirstEntityByDisplayName("TestCar2"));

    garageScriptComponent->CreateAndAddScript<GarageScript>(&garageParams);
    garageRoomEnt->addComponent(*garageScriptComponent);



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
    //auto carEnt = entityManager->getEntity(carID);

    // making a 2nd car
    auto carEnt2 = entityManager->findFirstEntityByDisplayName("TestCar2");
    //auto carEnt2 = entityManager->getEntity(car2ID);
    carEnt2->transform->setLocalPosition(glm::vec3(1.5f, 0.0f, 0.0f));
    carEnt2->transform->setLocalScale(glm::vec3(0.4f, 0.4f, 0.4f));
    
    /*
    OrbitScriptParams orbitParams;
    orbitParams.m_OrbitTarget = carEnt->transform;
    orbitParams.m_OrbitTarget2 = carEnt2->transform;
    cameraScriptComponent->AddScriptToComponent<OrbitScript>(&orbitParams);
    OrbitScript* orbitScript = cameraScriptComponent->GetScript<OrbitScript>();
    */

    /*
    input.RegisterKeyCallback(GLFW_KEY_SPACE, [orbitScript](Input::ActionType action) {
        if (action == Input::PRESS) {
            orbitScript->swapOrbitTargets();
        }
    });
    */
    // End Example

    //auto gameManagerEnt = entityManager->findFirstEntityByDisplayName("Game Manager");
    //auto gameManagerEnt = entityManager->getEntity(gameManagerID);

    ScriptComponent* timerScriptComponent = new ScriptComponent(gameManagerEnt);
    gameManagerEnt->addComponent(*timerScriptComponent);

    TimerScriptParams timerParams;
    //auto carEnt = entityManager->findFirstEntityByDisplayName("Test Car");
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
