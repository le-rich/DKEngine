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
    //createGameManager();
    
    //CameraEntity* cameraEntity = new CameraEntity();
    //cameraEntity->SetDisplayName("Main Camera");
    //cameraEntity->transform->setWorldPosition(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    //EntityManager::getInstance().Instantiate(cameraEntity);

    //LightEntity* lightEntity = new LightEntity();
    //lightEntity->SetDisplayName("Light");
    //lightEntity->transform->setLocalPosition(glm::vec3(0.f, 5.f, 0.f));
    //lightEntity->transform->setLocalOrientation(glm::quat(-0.5f, 0.5f, 0.5f, 0.f));
    //EntityManager::getInstance().Instantiate(lightEntity);

    //// TODO: JSONparser for Scene entities and scripts/components.
    //// TODO: Figure out location and pathing of assets/non code files within solution
    //const std::string SOURCE_FOLDER = "TestAE2/";
    //const std::string MODEL_FILE = "ae86.gltf";

    //Entity* testCar = new Entity();
    //testCar->SetDisplayName("Test Car");
    ////GLTFLoader::LoadModelAsEntity(testCar, SOURCE_FOLDER, MODEL_FILE);
    //EntityManager::getInstance().Instantiate(testCar);

    //Entity* gameManager = new Entity();
    //gameManager->SetDisplayName("Game Manager");
    //EntityManager::getInstance().Instantiate(gameManager);

    // Example of using findFirstEntityByDisplayName() for adding additional components
    //auto* entityManager = &(EntityManager::getInstance());
    //auto cameraID = entityManager->findFirstEntityByDisplayName("Main Camera");
    //auto cameraEnt = entityManager->getEntity(cameraID);

    //ScriptComponent* scriptComponent = new ScriptComponent(cameraEnt);
    //cameraEnt->addComponent(*scriptComponent);

    //Entity* carEnt = entityManager->findFirstEntityByDisplayName("Test Car");

    //OrbitScriptParams params;
    //params.m_OrbitTarget = carEnt->transform;
    //scriptComponent->CreateAndAddScript<OrbitScript>(&params);
    //// End Example

    //auto gameManagerEnt = entityManager->findFirstEntityByDisplayName("Game Manager");
    //auto gameManagerEnt = entityManager->getEntity(gameManagerID);
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
}
