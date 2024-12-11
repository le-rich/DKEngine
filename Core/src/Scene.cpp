#pragma once

#include "Scene.h"

#include "Entities/CameraEntity.h"
#include "Entities/LightEntity.h"
//#include "GLTFLoader.h"
#include "SceneParser.h"
#include "Managers/EntityManager.h"
#include "Managers/FMODManager.h"
#include "Components/MeshComponent.h"
#include "Components/ScriptComponent.h"
#include "Components/ListenerComponent.h"
#include "GarageScript.h"
#include "OrbitScript.h"
#include "TimerScript.h"
#include "LapManagerScript.h"
#include "LapCheckpointScript.h"
#include "FollowCamScript.h"
#include "CarControllerScript.h"
//#include "Input.h"

#include <vector>

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
    createGameManager();

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

    Entity* pbrSphere = entityManager->findFirstEntityByDisplayName("PBRSphere");
    if (pbrSphere != nullptr)
    {
        pbrSphere->transform->setWorldPosition(glm::vec4(-2, 1, 0, 1));
        pbrSphere->transform->setLocalScale(glm::vec4(0.5f, 0.5f, 0.5f, 1));
    }
    pbrSphere = entityManager->findFirstEntityByDisplayName("PBRSphere1");
    if (pbrSphere != nullptr)
    {
        pbrSphere->transform->setWorldPosition(glm::vec4(-1, 1, 0, 1));
        pbrSphere->transform->setLocalScale(glm::vec4(0.5f, 0.5f, 0.5f, 1));

    }
    pbrSphere = entityManager->findFirstEntityByDisplayName("PBRSphere2");
    if (pbrSphere != nullptr)
    {
        pbrSphere->transform->setWorldPosition(glm::vec4(0, 1, 0, 1));
        pbrSphere->transform->setLocalScale(glm::vec4(0.5f, 0.5f, 0.5f, 1));

        MeshComponent* meshComponent = dynamic_cast<MeshComponent*>(pbrSphere->getComponent(ComponentType::Mesh));
        if (meshComponent != nullptr)
        {
            auto albedo = AssetManager::GetInstance().GetTextureByName("rocky-rugged-terrain_1_albedo.png");
            auto ao = AssetManager::GetInstance().GetTextureByName("rocky-rugged-terrain_1_ao.png");
            auto height = AssetManager::GetInstance().GetTextureByName("rocky-rugged-terrain_1_height.png");
            auto metallic = AssetManager::GetInstance().GetTextureByName("rocky-rugged-terrain_1_metallic.png");
            auto normal = AssetManager::GetInstance().GetTextureByName("rocky-rugged-terrain_1_normal-ogl.png");
            auto roughness = AssetManager::GetInstance().GetTextureByName("rocky-rugged-terrain_1_roughness.png");

            std::vector<Primitive*> primitives = meshComponent->getPrimitives();

            std::shared_ptr<Material> newMaterial = std::make_shared<Material>(
                AssetManager::GetInstance().GetDefaultShader()->GetAssetID(),
                albedo->GetAssetID()
            );
            AssetManager::GetInstance().AddMaterial(newMaterial);
            auto materialUUID = newMaterial->GetAssetID();

            for (auto primitive : primitives)
            {
                primitive->mMaterialID = materialUUID;

                std::shared_ptr<Material> primMaterial = AssetManager::GetInstance().GetMaterialByID(materialUUID);
                primMaterial->SetTexture(0, albedo->GetAssetID());
                primMaterial->SetTexture(1, metallic->GetAssetID());
                primMaterial->SetTexture(2, normal->GetAssetID());
                primMaterial->SetTexture(3, height->GetAssetID());
                primMaterial->SetTexture(4, ao->GetAssetID());
                primMaterial->SetTexture(5, roughness->GetAssetID());
            }
        }
    }
    pbrSphere = entityManager->findFirstEntityByDisplayName("PBRSphere3");
    if (pbrSphere != nullptr)
    {
        pbrSphere->transform->setWorldPosition(glm::vec4(1, 1, 0, 1));
        pbrSphere->transform->setLocalScale(glm::vec4(0.5f, 0.5f, 0.5f, 1));
    }
    pbrSphere = entityManager->findFirstEntityByDisplayName("PBRSphere4");
    if (pbrSphere != nullptr)
    {
        pbrSphere->transform->setWorldPosition(glm::vec4(2, 1, 0, 1));
        pbrSphere->transform->setLocalScale(glm::vec4(0.5f, 0.5f, 0.5f, 1));
    }

    // TODO: We can have the GarageManager create/handle the orbit script and/or this is a State toggled by GameManager
    // spin
    Entity* cameraEnt = entityManager->findFirstEntityByDisplayName("Main Camera");
    ScriptComponent* orb = dynamic_cast<ScriptComponent*>(cameraEnt->getComponent(ComponentType::Script));
    OrbitScript* orbScript = orb->GetScript<OrbitScript>();

    // Audio setup, Car Noise + Camera Listener
    // FMODManager* audioManager = FMODManager::GetInstance();
    // FMOD::Sound* carMotorSound = audioManager->LoadAudio("Assets/Audio/car-motor.mp3");
    // AudioComponent* carAudio = new AudioComponent(carEnt);
    // carEnt->addComponent(*carAudio);
    // carAudio->PlaySound(carMotorSound, true, false);
    
    // ListenerComponent* listener = new ListenerComponent(cameraEnt, 0);
    // cameraEnt->addComponent(*listener);

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
    auto* entityManager = &(EntityManager::getInstance());

    Entity* gameManagerEnt = new Entity();
    gameManagerEnt->SetDisplayName("GameManager");
    entityManager->Instantiate(gameManagerEnt);

    ScriptComponent* gameManagerScriptComponent = new ScriptComponent(gameManagerEnt);
    gameManagerEnt->addComponent(*gameManagerScriptComponent);

    auto carEnt = entityManager->findFirstEntityByDisplayName("Test Car");
    TimerScriptParams timerParams;
    timerParams.m_TimerTarget = carEnt->transform;
    timerParams.m_OriginalPosition = carEnt->transform->getWorldPosition();
    gameManagerScriptComponent->CreateAndAddScript<TimerScript>(&timerParams);

    // Create Lap Manager
    LapManagerScriptParams lapManagerParams;
    gameManagerScriptComponent->CreateAndAddScript<LapManagerScript>(&lapManagerParams);
    LapManagerScript* lapManagerPointerFromScript = gameManagerScriptComponent->GetScript<LapManagerScript>();

    // Create Checkpoints
    LapCheckpointScriptParams params;
    Entity* targetEntity = EntityManager::getInstance().findFirstEntityByDisplayName("Test Car");
    std::vector<LapCheckpointScript*> checkpoints;
    for (int i = 1; ; ++i)
    {
        auto checkpoint = entityManager->findFirstEntityByDisplayName("Checkpoint " + std::to_string(i));
        if (!checkpoint) break;
        params.m_Index = i;
        auto checkpointScriptComponent = dynamic_cast<ScriptComponent*>(checkpoint->getComponent(ComponentType::Script));
        if (!checkpointScriptComponent)
        {
            checkpointScriptComponent = new ScriptComponent(checkpoint);
            checkpoint->addComponent(*checkpointScriptComponent);
        }
        checkpointScriptComponent->CreateAndAddScript<LapCheckpointScript>(&params);

        auto checkpointScript = checkpointScriptComponent->GetScript<LapCheckpointScript>();
        lapManagerParams.m_Checkpoints.push_back(checkpointScript);
        checkpointScript->SetLapManager(lapManagerPointerFromScript);
    }

    // Add Checkpoints to LapManager
    lapManagerPointerFromScript->SetParameters(&lapManagerParams);
}

Entity* Scene::GetSceneCopy()
{
    Entity* cur = new Entity(*sceneRoot);
    return cur;
}
