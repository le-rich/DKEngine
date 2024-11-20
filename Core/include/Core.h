#pragma once

#include <vector>
#include <iostream>

class Scene;
class System;

class Core {
private:
	std::vector<System*> systems;
	Scene* currentScene = nullptr;

	// Private constructor as this should be a singleton.
    Core(){

    }


public:
	static Core& getInstance() {
		static Core instance;
		return instance;
	}

	void AddSystem(System* system);
	std::vector<System*> GetSystems();
	void Synchronize();
	void SetScene(Scene* scene);
	Scene* GetScene() const;
};

