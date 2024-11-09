#include "Managers/AudioManager.h"
#include "iostream"

AudioManager::AudioManager()
{
	FMOD::System_Create(&fmodSystem);
	fmodSystem->init(512, FMOD_INIT_3D_RIGHTHANDED, nullptr);
}

AudioManager::~AudioManager()
{
	for (auto& pair : soundCache)
	{
		pair.second->release();
	}
	fmodSystem->release();
}

void AudioManager::update()
{
	fmodSystem->update();
}

FMOD::System* AudioManager::getSystem() {
	return fmodSystem;
}

FMOD::Sound* AudioManager::loadSound(const std::string& filePath) {
	auto it = soundCache.find(filePath);
	if (it != soundCache.end()) {
		return it->second;
	}

	FMOD::Sound* newSound;
	fmodSystem->createSound(filePath.c_str(), FMOD_3D, nullptr, &newSound);
	soundCache[filePath] = newSound;
	return newSound;
}

void AudioManager::setListenerAttributes(const FMOD_VECTOR& position, const FMOD_VECTOR& forward, const FMOD_VECTOR& up) {
	FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f }; // TODO: get velocity for doppler
	fmodSystem->set3DListenerAttributes(0, &position, &velocity, &forward, &up);
}

void AudioManager::setMasterVolume(float volume) {
	fmodSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_STEREO, 0);
	fmodSystem->setSoftwareChannels(100);
}

void AudioManager::playSound(const std::string& soundName) {

	
	auto it = soundCache.find(soundName);
	if (it != soundCache.end()) {
		FMOD::Channel* newChannel = nullptr;
		fmodSystem->playSound(it->second, nullptr, false, &newChannel);
	} else {
		std::cerr << "Sound not found: " << soundName << std::endl;
	}
}