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

    LightEntity* lightEntity = new LightEntity();
    lightEntity->SetDisplayName("Light");
    lightEntity->transform->setLocalPosition(glm::vec3(0.f, 5.f, 0.f));
    lightEntity->transform->setLocalOrientation(glm::quat(-0.5f, 0.5f, 0.5f, 0.f));
    EntityManager::getInstance().Instantiate(lightEntity);
    auto* entityManager = &(EntityManager::getInstance());

    Entity* cameraEnt = entityManager->findFirstEntityByDisplayName("Main Camera");

    // TODO: JSONparser for Scene entities and scripts/components.
    // TODO: Figure out location and pathing of assets/non code files within solution

    const std::string MAP_SOURCE_FOLDER = "TestMap/";
    const std::string MAP_MODEL_FILE = "track.gltf";

    Entity* raceTrackEnt = new Entity();
    raceTrackEnt->SetDisplayName("Race Track");
    GLTFLoader::LoadModelAsEntity(raceTrackEnt, MAP_SOURCE_FOLDER, MAP_MODEL_FILE);
    EntityManager::getInstance().Instantiate(raceTrackEnt);

    raceTrackEnt->transform->setWorldPosition(glm::vec4(0.0f, -1.3f, 0.0f, 1.0f));

    Entity* carEnt = entityManager->findFirstEntityByDisplayName("Test Car");

    // TODO: make care controller params set-up method instead to clean up scene.cpp
    RigidBodyComponent* rigidBodyComponent = new RigidBodyComponent(carEnt,
        std::shared_ptr<AE86::RigidBody>(new AE86::RigidBody()),
        1.0f);
    carEnt->addComponent(*rigidBodyComponent);
    ScriptComponent* carScriptComponent = new ScriptComponent(carEnt);
    carEnt->addComponent(*carScriptComponent);
    CarControllerScriptParams carParams;
    // these are all stats for an AE86 (with a little bit of modification):
    carParams.m_InverseMass =  1.0f / 1500.0f;
    carParams.m_Height = 1.335f;
    carParams.m_Width = 1.625f;
    carParams.m_Length = 4.185f;
    carParams.m_CGToFrontAxleDistance = 1.25f;
    carParams.m_CGToRearAxleDistance = 1.25f;
    carParams.m_CGHeight = 1.0f;
    carParams.m_WheelBase = 2.50f;
    carParams.m_WheelLength = 0.7f;
    carParams.m_WheelWidth = 0.3f;
    carParams.m_FrictionCoeff = 0.30f;
    carParams.m_FrontalArea = 2.2f;
    carParams.m_EngineForce = 8000.0f;
    carParams.m_BrakeForce = 12000.0f;
    carParams.m_DifferentialRatio = 3.42;
    carParams.m_WheelRadius = 0.33f;
    carParams.m_CorneringStiffness = -5.20f;
    carParams.m_TireGrip = 2.0f;
    carScriptComponent->CreateAndAddScript<CarControllerScript>(&carParams);
    carScriptComponent->GetScript<CarControllerScript>()->SetUpInput();

    ScriptComponent* cameraScriptComponent = dynamic_cast<ScriptComponent*>(cameraEnt->getComponent(ComponentType::Script));
    FollowCamScriptParams followCamScriptParams;
    followCamScriptParams.m_FollowTarget = carEnt->transform;
    cameraScriptComponent->CreateAndAddScript<FollowCamScript>(&followCamScriptParams);


    const std::string CUBE_SOURCE_FOLDER = "TestCube2/";
    const std::string CUBE_MODEL_FILE = "testcube.gltf";

    // TODO: move waypoint visual creation to the lapcheckpointscripts
    Entity* cubeEntity = new Entity();
    GLTFLoader::LoadModelAsEntity(cubeEntity, CUBE_SOURCE_FOLDER, CUBE_MODEL_FILE);
    EntityManager::getInstance().Instantiate(cubeEntity);
    Entity* checkpoint1 = EntityManager::getInstance().findFirstEntityByDisplayName("Checkpoint 1");
    checkpoint1->addChild(cubeEntity);
    cubeEntity->setParent(checkpoint1);
    EntityManager::getInstance().addEntityToMap(*cubeEntity);

    Entity* checkpoint2 = EntityManager::getInstance().findFirstEntityByDisplayName("Checkpoint 2");
    cubeEntity = new Entity();
    GLTFLoader::LoadModelAsEntity(cubeEntity, CUBE_SOURCE_FOLDER, CUBE_MODEL_FILE);
    EntityManager::getInstance().Instantiate(cubeEntity);
    checkpoint2->addChild(cubeEntity);
    cubeEntity->setParent(checkpoint2);
    EntityManager::getInstance().addEntityToMap(*cubeEntity);


    // making a 2nd car
    //Entity* testCar2 = new Entity();
    //testCar2->SetDisplayName("Test Car 2");
    //GLTFLoader::LoadModelAsEntity(testCar2, CAR_2_SOURCE_FOLDER, CAR_2_MODEL_FILE);
    //EntityManager::getInstance().Instantiate(testCar2);
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
    
    RigidBodyComponent* rigidBodyComponent = new RigidBodyComponent(carEnt, 
        std::shared_ptr<AE86::RigidBody>(new AE86::RigidBody()), 
       1.0f);
    carEnt->addComponent(*rigidBodyComponent);

    ScriptComponent* carScriptComponent = new ScriptComponent(carEnt);
    carEnt->addComponent(*carScriptComponent);
    //CarControllerScriptParams carParams;
    //carParams.m_InverseMass = 0.95f;
    //carParams.m_Height = 1.335f;
    //carParams.m_Width = 1.625f;
    //carParams.m_Length = 4.185f;
    //carScriptComponent->AddScriptToComponent<CarControllerScript>(&carParams);

    // making a 2nd car
    auto carEnt2 = entityManager->findFirstEntityByDisplayName("Body");
    carEnt2->transform->setLocalPosition(glm::vec3(1.5f, 0.0f, 0.0f));
    carEnt2->transform->setLocalScale(glm::vec3(0.4f, 0.4f, 0.4f));
    

    OrbitScriptParams orbitParams;
    orbitParams.m_OrbitTarget = carEnt->transform;
    orbitParams.m_OrbitTarget2 = carEnt2->transform;
    //cameraScriptComponent->AddScriptToComponent<OrbitScript>(&orbitParams);
    //OrbitScript* orbitScript = cameraScriptComponent->GetScript<OrbitScript>();

    // Garage Room
    //Entity* garageRoomController = new Entity();
    //garageRoomController->SetDisplayName("Garage Controller");
    //EntityManager::getInstance().Instantiate(garageRoomController);

    //auto garageRoomID = entityManager->findFirstEntityByDisplayName("Garage Controller");
    //auto garageRoomEnt = entityManager->getEntity(garageRoomID);
    //ScriptComponent* garageScriptComponent = new ScriptComponent(garageRoomEnt);
    //garageRoomEnt->addComponent(*garageScriptComponent);

    /*
    input.RegisterKeyCallback(GLFW_KEY_SPACE, [orbitScript](Input::ActionType action) {
        if (action == Input::PRESS) {
            orbitScript->swapOrbitTargets();
        }
    });
    */
    // End Example


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
