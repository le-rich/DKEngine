#include "Scene.h"

#include "Entities/CameraEntity.h"
#include "Managers/EntityManager.h"

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
    EntityManager::getInstance().Instantiate(cameraEntity);


}
