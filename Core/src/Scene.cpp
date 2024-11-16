#include "Scene.h"

#include "Entities/CameraEntity.h"
#include "Entities/LightEntity.h"
#include "GLTFLoader.h"
#include "Managers/EntityManager.h"
#include "Components/MeshComponent.h"
#include "Components/ScriptComponent.h"
#include "OrbitScript.h"
#include "TimerScript.h"
#include "LapColliderScript.h"
#include "LapManagerScript.h"

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

    // TODO: JSONparser for Scene entities and scripts/components.
    // TODO: Figure out location and pathing of assets/non code files within solution
    const std::string SOURCE_FOLDER = "TestAE2/";
    const std::string MODEL_FILE = "ae86.gltf";

    Entity* testCar = new Entity();
    testCar->SetDisplayName("Test Car");
    GLTFLoader::LoadModelAsEntity(testCar, SOURCE_FOLDER, MODEL_FILE);
    EntityManager::getInstance().Instantiate(testCar);

    Entity* gameManager = new Entity();
    gameManager->SetDisplayName("Game Manager");
    EntityManager::getInstance().Instantiate(gameManager);

    // Example of using findFirstEntityByDisplayName() for adding additional components
    auto* entityManager = &(EntityManager::getInstance());
    auto cameraID = entityManager->findFirstEntityByDisplayName("Main Camera");
    auto cameraEnt = entityManager->getEntity(cameraID);

    ScriptComponent* cameraScriptComponent = new ScriptComponent(cameraEnt);
    cameraEnt->addComponent(*cameraScriptComponent);
    // Add lap counter entity
    // Create Lap Manager Entity
    Entity* lapManagerEntity = new Entity();
    lapManagerEntity->SetDisplayName("Lap Manager");
    ScriptComponent* lapManagerComponent = new ScriptComponent(lapManagerEntity);
    LapManagerScript* lapManagerScript = new LapManagerScript(lapManagerEntity, 2);

    // Add 1st lap trigger entity with script component
    Entity* lapTriggerEntity1 = new Entity();
    lapTriggerEntity1->SetDisplayName("Lap Trigger 1");
    ScriptComponent* lapTriggerComponent1 = new ScriptComponent(lapTriggerEntity1);
    LapColliderScript* lapColliderScript1 = new LapColliderScript(lapTriggerEntity1);
    lapManagerScript->RegisterCheckpoint(lapColliderScript1); // Register the collider script
    lapColliderScript1->SetOther(testCar->transform);
    lapTriggerComponent1->AddScript<LapColliderScript>(*lapColliderScript1);    
    lapTriggerEntity1->addComponent(*lapTriggerComponent1);
    lapTriggerEntity1->transform->setWorldPosition(glm::vec4(-1.5f, 0.f, 0.f, 1.0f));
    EntityManager::getInstance().Instantiate(lapTriggerEntity1);

    lapManagerComponent->AddScript<LapManagerScript>(*lapManagerScript);
    lapManagerEntity->addComponent(*lapManagerComponent);

    // Init Lap Manager after checkpoints entities are created and assigned
    EntityManager::getInstance().Instantiate(lapManagerEntity);

    auto carID = entityManager->findFirstEntityByDisplayName("Test Car");
    auto carEnt = entityManager->getEntity(carID);

    OrbitScriptParams orbitParams;
    orbitParams.m_OrbitTarget = carEnt->transform;
    cameraScriptComponent->AddScriptToComponent<OrbitScript>(&orbitParams);
    // End Example

    auto gameManagerID = entityManager->findFirstEntityByDisplayName("Game Manager");
    auto gameManagerEnt = entityManager->getEntity(gameManagerID);

    ScriptComponent* timerScriptComponent = new ScriptComponent(gameManagerEnt);
    gameManagerEnt->addComponent(*timerScriptComponent);

    TimerScriptParams timerParams;
    timerParams.m_TimerTarget = carEnt->transform;
    timerParams.m_OriginalPosition = carEnt->transform->getWorldPosition();
    timerScriptComponent->AddScriptToComponent<TimerScript>(&timerParams);
}

Entity* Scene::GetSceneCopy()
{
    Entity* cur = new Entity(*sceneRoot);
    return cur;
}
