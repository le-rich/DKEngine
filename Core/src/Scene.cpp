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

    const std::string MAP_SOURCE_FOLDER = "TestMap/";
    const std::string MAP_MODEL_FILE = "track.gltf";

    Entity* testCar = new Entity();
    testCar->SetDisplayName("Test Car");
    GLTFLoader::LoadModelAsEntity(testCar, SOURCE_FOLDER, MODEL_FILE);
    EntityManager::getInstance().Instantiate(testCar);

    Entity* testMap = new Entity();
    testMap->SetDisplayName("Test Map");
    GLTFLoader::LoadModelAsEntity(testMap, MAP_SOURCE_FOLDER, MAP_MODEL_FILE);
    testMap->transform->setLocalPosition(glm::vec3(0.0f, -1.3f, 0.0f));
    EntityManager::getInstance().Instantiate(testMap);

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
