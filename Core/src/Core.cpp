#include "Core.h"

#include "Scene.h"
#include "System.h"

void Core::AddSystem(System* system) {
	systems.push_back(system);
}

std::vector<System*> Core::GetSystems() {
	return this->systems;
}

void Core::Synchronize() {

}

void Core::SetScene(Scene* scene) {
	if (scene != nullptr) {
		currentScene = scene;
	}
}

Scene* Core::GetScene() const {
	return currentScene;
}
