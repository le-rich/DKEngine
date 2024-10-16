#pragma

#include "Scene.h"
#include "System.h"

class Core {
private:
	std::vector<System*> systems;
	Scene* currentScene;

	// Private constructor as this should be a singleton.
    Core(){
        std::cout << "Core" << std::endl;
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

