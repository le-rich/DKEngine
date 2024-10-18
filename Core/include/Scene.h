#pragma once
#include "Entity.h"

class Scene {
public:
	Scene();
	~Scene();

	Entity* sceneRoot = nullptr;

	// TODO: This function loads and structures the scene graph. This should be made virtual only.
	void SpawnSceneDefinition();
private:

};