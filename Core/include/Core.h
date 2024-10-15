#pragma

#include "Scene.h"
#include "System.h"

class Core {
private:
	std::vector<System*> systems;
	Scene* currentScene;

public:
	static Core& getInstance();
	void AddSystem(System* system);
	std::vector<System*> GetSystems();
	void Synchronize();
	void SetScene(Scene* scene);
	Scene* GetScene() const;
};

