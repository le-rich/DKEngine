#include "Scene.h"

#include "Entities/CameraEntity.h"
#include "Entities/LightEntity.h"
//#include "GLTFLoader.h"
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
    //GLTFLoader::LoadModelAsEntity(testCar, SOURCE_FOLDER, MODEL_FILE);
    EntityManager::getInstance().Instantiate(testCar);

    // Example of using findFirstEntityByDisplayName() for adding additional components
    auto* entityManager = &(EntityManager::getInstance());
    Entity* cameraEnt = entityManager->findFirstEntityByDisplayName("Main Camera");

    ScriptComponent* scriptComponent = new ScriptComponent(cameraEnt);
    cameraEnt->addComponent(*scriptComponent);

    Entity* carEnt = entityManager->findFirstEntityByDisplayName("Test Car");

    OrbitScriptParams params;
    params.m_OrbitTarget = carEnt->transform;
    scriptComponent->CreateAndAddScript<OrbitScript>(&params);
    // End Example


}
