#include <vector>

#include "Scene.h"

#include "Entities/CameraEntity.h"
#include "Entities/LightEntity.h"
#include "GLTFLoader.h"
#include "Managers/EntityManager.h"
#include "Components/MeshComponent.h"
#include "Components/ScriptComponent.h"
#include "OrbitScript.h"
#include "TimerScript.h"
#include "LapManagerScript.h"
#include "LapCheckpointScript.h"

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

    // Create 1st checkpoint
    Entity* lapCheckpointEntity1 = new Entity();
    lapCheckpointEntity1->SetDisplayName("Lap Checkpoint 1");
    ScriptComponent* lapCheckpointComponent1 = new ScriptComponent(lapCheckpointEntity1);
    LapCheckpointScript* lapCheckpointScript1 = new LapCheckpointScript(lapCheckpointEntity1);
    lapCheckpointScript1->Init(lapManagerScript, testCar->transform, 1);
    lapCheckpointComponent1->AddScript<LapCheckpointScript>(*lapCheckpointScript1);
    lapCheckpointEntity1->addComponent(*lapCheckpointComponent1);
    lapCheckpointEntity1->transform->setWorldPosition(glm::vec4(-0.75f, 0.f, 0.f, 1.0f));

    // Create 2nd checkpoint
    Entity* lapCheckpointEntity2 = new Entity();
    lapCheckpointEntity2->SetDisplayName("Lap Checkpoint 2");
    ScriptComponent* lapCheckpointComponent2 = new ScriptComponent(lapCheckpointEntity2);
    LapCheckpointScript* lapCheckpointScript2 = new LapCheckpointScript(lapCheckpointEntity2);
    lapCheckpointScript2->Init(lapManagerScript, testCar->transform, 2);
    lapCheckpointComponent2->AddScript<LapCheckpointScript>(*lapCheckpointScript2);
    lapCheckpointEntity2->addComponent(*lapCheckpointComponent2);
    lapCheckpointEntity2->transform->setWorldPosition(glm::vec4(-1.75f, 0.f, 0.f, 1.0f));

    // Set checkpoints
    std::vector<LapCheckpointScript*> checkpoints = { lapCheckpointScript1, lapCheckpointScript2 };
    lapManagerScript->SetCheckpoints(checkpoints);

    // Init Lap Manager after checkpoints are created
    lapManagerComponent->AddScript<LapManagerScript>(*lapManagerScript);
    lapManagerEntity->addComponent(*lapManagerComponent);

    EntityManager::getInstance().Instantiate(lapCheckpointEntity1);
    EntityManager::getInstance().Instantiate(lapCheckpointEntity2);
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
