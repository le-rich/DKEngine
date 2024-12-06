#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "System.h"
#include <Components/TransformComponent.h>

#include <glm/vec3.hpp>
#include <fmod.hpp>

#include <unordered_map>
#include <string>

class AudioManager : public System {
public:
	AudioManager();

	~AudioManager();

	static AudioManager& GetInstance() {
		static AudioManager instance;
		return instance;
	}
	
	const char* GetName() const override {
		return "AudioManager";
	};

	void Initialize() override;
	FMOD::System* GetSystem();
	FMOD::Sound* LoadAudio(const std::string& filePath);
	void Update(float deltaTime) override;
	void FixedUpdate() override;
	void updateListenerPosition(Entity* listenerParent);
	static FMOD_VECTOR GetFMODVector(const glm::vec3& position);
private:
	glm::vec3 listenerPosition;
	FMOD::System* fmodSystem;
	std::unordered_map<std::string, FMOD::Sound*> soundCache;
};

#endif // AUDIOMANAGER_H