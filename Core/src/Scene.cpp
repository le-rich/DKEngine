#include "Scene.h"
#include "EntityManager.h"

Scene::Scene() {
	sceneRoot = new Entity("root");
	EntityManager::getInstance().addEntityToMap(sceneRoot);
}

Scene::~Scene() {

}