#include "Managers/AudioManager.h"
#include <iostream>

AudioManager::AudioManager() {
    FMOD::System_Create(&fmodSystem);
    listenerPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    soundPosition = glm::vec3(0.0f, 0.0f, 0.0f);
}

void AudioManager::Initialize()
{
    fmodSystem->init(512, FMOD_INIT_3D_RIGHTHANDED, nullptr);
}

AudioManager::~AudioManager() {
    for (auto& pair : soundCache) {
        pair.second->release();
    }
    fmodSystem->release();
}

FMOD::Sound* AudioManager::LoadAudio(const std::string& filePath) {
    auto it = soundCache.find(filePath);
    if (it != soundCache.end()) {
        return it->second;
    }

    FMOD::Sound* newSound;
    FMOD_RESULT result = fmodSystem->createSound(filePath.c_str(), FMOD_3D, nullptr, &newSound);
    if (result != FMOD_OK) {
        std::cerr << "Failed to load sound: " << filePath << std::endl;
        return nullptr;
    }

    soundCache[filePath] = newSound;
    return newSound;
}

void AudioManager::FixedUpdate() {}

void AudioManager::PlayStaticSound(FMOD::Sound* sound, bool isLooping, const glm::vec3& position) {
    PlaySound(sound, isLooping, position, staticChannel);
}

void AudioManager::PlayDynamicSound(FMOD::Sound* sound, bool isLooping, const glm::vec3& position) {
    PlaySound(sound, isLooping, position, dynamicChannel);
}

void AudioManager::PlaySound(FMOD::Sound* sound, bool isLooping, const glm::vec3& position, FMOD::Channel* channel) {
    
    if (!sound) return;
    if (isLooping)
        sound->setMode(FMOD_LOOP_NORMAL);

    FMOD_RESULT result = fmodSystem->playSound(sound, nullptr, false, &channel);
    if (result == FMOD_OK && channel) {
        channel->setLoopCount(-1);
        channel->setVolume(1.0f);
        FMOD_VECTOR audioPosition = GetFMODVector(soundPosition);
        channel->set3DAttributes(&audioPosition, nullptr);
        channel->setPaused(false);
    }
}


// void AudioManager::UpdateChannelPosition(FMOD::Channel* channel, const glm::vec3& position) {
//     if (!channel) return;
//
//     FMOD_VECTOR fmodPosition = GetFMODVector(position);
//     channel->set3DAttributes(&fmodPosition, nullptr);
// }

FMOD_VECTOR AudioManager::GetFMODVector(const glm::vec3& position) {
    return { position.x, position.y, position.z };
}

// TODO: Positioning of Camera , glm::vec3 cameraPosition, glm::vec3& carPosition
void AudioManager::Update(float deltaTime) {
    // get and update positions and velocities of each channel
    
    FMOD_VECTOR FMODListenerPosition = GetFMODVector(listenerPosition); // Listener at origin
    FMOD_VECTOR audioPosition = GetFMODVector(soundPosition);
    fmodSystem->set3DListenerAttributes(0, &FMODListenerPosition, nullptr, nullptr, nullptr);
    dynamicChannel->set3DAttributes(&audioPosition, nullptr);
    fmodSystem->update();
}

FMOD::System* AudioManager::GetSystem() {
    return fmodSystem;
}

void AudioManager::updateListenerPosition(glm::vec3 position) {
    listenerPosition.x = position.x;
    listenerPosition.y = position.y;
    listenerPosition.z = position.z;

}

void AudioManager::updateSoundPosition(glm::vec3 position) {
    soundPosition.x = position.x;
    soundPosition.y = position.y;
    soundPosition.z = position.z;
}
