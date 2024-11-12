#include "Scene.h"

#include "Entities/CameraEntity.h"
#include "Entities/LightEntity.h"
#include "GLTFLoader.h"
#include "Managers/EntityManager.h"
#include "Components/MeshComponent.h"
#include "Components/ScriptComponent.h"
#include "OrbitScript.h"

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

    sceneRoot->addChild(cameraEntity);

    LightEntity* lightEntity = new LightEntity();
    lightEntity->SetDisplayName("Light");
    lightEntity->transform->setLocalPosition(glm::vec3(0.f, 5.f, 0.f));
    lightEntity->transform->setLocalOrientation(glm::quat(-0.5f, 0.5f, 0.5f, 0.f));
    EntityManager::getInstance().Instantiate(lightEntity);

    // TODO: Refactor to a Single Car Entity with Mesh and Rigidbody components
    // 
    // TODO: Figure out location of assets/non code files within solution
    // TODO: JSONparser for list of assets, each asset has a PATH and FILE string.
    const std::string SOURCE_FOLDER = "Assets/TestAE2/";
    const std::string SOURCE_FILE = "ae86.gltf";

    Entity* testCar = new Entity();
    testCar->SetDisplayName("Test Car");
    GLTFLoader::LoadModelAsEntity(testCar, SOURCE_FOLDER, SOURCE_FILE);
    EntityManager::getInstance().Instantiate(testCar);
    // end TODO

    OrbitScriptParams params;
    params.m_OrbitTarget = testCar->transform;
    ScriptComponent* component = ComponentManager::AddScriptComponent(cameraEntity);
    ComponentManager::AddScriptToComponent(cameraEntity, component, ScriptType::OrbitScript, &params);

    EntityManager::getInstance().Instantiate(cameraEntity);
}
