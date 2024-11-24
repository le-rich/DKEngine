#ifndef AUDIO_MGR_H
#define AUDIO_MGR_H

#include "vec3.hpp"
#include "FMOD/inc/fmod.hpp"
#include <string>
#include <unordered_map>
#include "System.h" 

class AudioManager : public System {
public:
	
	AudioManager();
	
	~AudioManager();

	static AudioManager& GetInstance()
	{
		static AudioManager instance;
		return instance;
	}
	
	const char* GetName() const override {
		return "AudioManager";
	};
	
	FMOD::System* GetSystem();
	void Update(float deltaTime);
	void FixedUpdate() override;
	FMOD::Sound* LoadAudio(const std::string& filePath);
	void SetListenerAttributes(const FMOD_VECTOR& position, const FMOD_VECTOR& forward, const FMOD_VECTOR& up);
	void SetMasterVolume(float volume);
	void PlaySound(const std::string& soundName);
	FMOD::Sound* GetSound(const std::string& fileName);

private:
	FMOD::System* fmodSystem = nullptr;
	std::unordered_map<std::string, FMOD::Sound*> soundCache;
	
	// 3D sound attributes
	FMOD_VECTOR position = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
};

// Convert this kind of structure to ECS Components
//struct AudioComponent {
//    std::string filePath;
//    FMOD::Sound* sound = nullptr;
//    FMOD::Channel* channel = nullptr;
//    bool isPlaying = false;
//
//    // 3D sound attributes
//    FMOD_VECTOR position = { 0.0f, 0.0f, 0.0f };
//    FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
//};
// 
// I assume they need some kind of update to their shit.
// 
// FMOD::System::init will need to set FMOD_INIT_3D_RIGHTHANDED
// because FMOD uses the same coordinate system as DirectX, so we need to specify Right.
//
//struct ListenerComponent {
//    FMOD_VECTOR position = { 0.0f, 0.0f, 0.0f };
//    FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
//    FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
//};
// Not sure about a listener component, might just use the audio manager as the listener.
// Alternatively, we can di
//


#endif