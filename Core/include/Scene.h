#pragma once
#include "Entity.h"

class Scene {
public:
	Scene();
	~Scene();

	Entity* sceneRoot = nullptr;
private:

};