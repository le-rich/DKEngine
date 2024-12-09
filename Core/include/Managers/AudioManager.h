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
	~AudioManager();

	static AudioManager* GetInstance();
	
	const char* GetName() const override {
		return "AudioManager";
	};

	void Initialize() override;
	FMOD::System* GetSystem();
	FMOD::Sound* LoadAudio(const std::string& filePath);
	void Update(float deltaTime) override;
	void FixedUpdate() override;
	static FMOD_VECTOR GetFMODVector(const glm::vec3& position);
private:
	AudioManager();
	FMOD::System* fmodSystem;
	std::unordered_map<std::string, FMOD::Sound*> soundCache;
	AudioManager(const AudioManager& lc) = delete;
	AudioManager& operator= (const AudioManager& lc) = delete;

	static AudioManager* pInstance;
	static std::mutex mutex_;
};

#endif // AUDIOMANAGER_H