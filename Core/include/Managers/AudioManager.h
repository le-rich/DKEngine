#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "FMOD/inc/fmod.hpp"
#include <unordered_map>
#include <string>
#include <glm/vec3.hpp>

#include "System.h"
#include <Components/TransformComponent.h>

class AudioManager : public System {
friend class AudioComponent;
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
	
private:
	static FMOD_VECTOR GetFMODVector(const glm::vec3& position);
	glm::vec3 listenerPosition;
	FMOD::System* fmodSystem;
	std::unordered_map<std::string, FMOD::Sound*> soundCache;
};

#endif // AUDIOMANAGER_H