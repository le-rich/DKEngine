#ifndef AUDIO_MGR_H

#include "FMOD/inc/fmod.hpp"
#include <string>
#include <unordered_map>

class AudioManager {
public:
	AudioManager();
	~AudioManager();

	void update();
	FMOD::System* getSystem();

	FMOD::Sound* loadSound(const std::string& filePath);

	// IMPL fmodSystem->set3DListenerAttributes(0, &position, nullptr, &forward, &up);
	// OR we have a private variable representing the current listener and forward
	// to the same position as 0, not sure.
	// Get velocity of listener so we can use it to have doppler.
	// forward and up necessary for automated spatial audio.
	void setListenerAttributes(const FMOD_VECTOR& position, const FMOD_VECTOR& forward, const FMOD_VECTOR& up);

		void setMasterVolume(float volume);
private:
	FMOD::System* fmodSystem = nullptr;
	std::unordered_map<std::string, FMOD::Sound*> soundCache;
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