#include <vector>

#include "Scene.h"

#include "Entities/CameraEntity.h"
#include "Entities/LightEntity.h"
//#include "GLTFLoader.h"
#include "SceneParser.h"
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
    SceneParser::LoadScene(SCENE_FILE);
    //createGameManager(); // GameManager currently setup via SceneJSON
}

void Scene::createGameManager()
{
    Entity* gameManager = new Entity();
    gameManager->SetDisplayName("GameManager");
    EntityManager::getInstance().Instantiate(gameManager);

    auto* entityManager = &(EntityManager::getInstance());
    auto gameManagerEnt = entityManager->findFirstEntityByDisplayName("GameManager");

    ScriptComponent* timerScriptComponent = new ScriptComponent(gameManagerEnt);
    gameManagerEnt->addComponent(*timerScriptComponent);

    TimerScriptParams timerParams;
    auto carEnt = entityManager->findFirstEntityByDisplayName("Test Car");
    timerParams.m_TimerTarget = carEnt->transform;
    timerParams.m_OriginalPosition = carEnt->transform->getWorldPosition();
    timerScriptComponent->CreateAndAddScript<TimerScript>(&timerParams);

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
    checkpointComponent1->CreateAndAddScript<LapCheckpointScript>(&checkpointParams1);
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
    checkpointComponent2->CreateAndAddScript<LapCheckpointScript>(&checkpointParams2);
    checkpointEntity2->addComponent(*checkpointComponent2);
    
    // Set LapManagerScript parameters & add to component
    LapCheckpointScript* checkpointPointerFromScript = checkpointComponent1->GetScript<LapCheckpointScript>();
    LapCheckpointScript* checkpointPointerFromScript2 = checkpointComponent2->GetScript<LapCheckpointScript>();
    LapManagerScriptParams lapManagerParams;
    std::vector<LapCheckpointScript*> checkpoints = { checkpointPointerFromScript, checkpointPointerFromScript2 };
    lapManagerParams.m_Checkpoints = checkpoints;
    lapManagerComponent->CreateAndAddScript<LapManagerScript>(&lapManagerParams);
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
