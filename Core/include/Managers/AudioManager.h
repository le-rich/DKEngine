#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "FMOD/inc/fmod.hpp"
#include <unordered_map>
#include <string>
#include <glm/vec3.hpp>

#include "System.h"
#include <Components/TransformComponent.h>

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
	void PlaySound(FMOD::Sound* sound, bool isLooping, const glm::vec3& position, FMOD::Channel* channel);
	void UpdateChannelPosition(FMOD::Channel* channel, const glm::vec3& position);
	void Update(float deltaTime) override;
	void FixedUpdate() override;

	void PlayStaticSound(FMOD::Sound* sound, bool isLooping, const glm::vec3& position);
	void PlayDynamicSound(FMOD::Sound* sound, bool isLooping, const glm::vec3& position);
	void updateListenerPosition(glm::vec3 position);
	void updateSoundPosition(glm::vec3 position);
	
private:
	FMOD_VECTOR GetFMODVector(const glm::vec3& position);
	FMOD::Channel* dynamicChannel = nullptr;
	FMOD::Channel* staticChannel = nullptr;
	glm::vec3 soundPosition;
	glm::vec3 listenerPosition;
	FMOD::System* fmodSystem;
	std::unordered_map<std::string, FMOD::Sound*> soundCache;
};

#endif // AUDIOMANAGER_H