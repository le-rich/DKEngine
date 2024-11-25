#include "Managers/AudioManager.h"
#include <iostream>

AudioManager::AudioManager() {
    FMOD::System_Create(&fmodSystem);
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


void AudioManager::PlaySound(FMOD::Sound* sound, bool isLooping, FMOD::Channel*& channel, const glm::vec3& position) {
    if (!sound) return;

    FMOD_RESULT result = fmodSystem->playSound(sound, nullptr, false, &channel);
    if (result == FMOD_OK && channel) {
        if (isLooping) {
            sound->setMode(FMOD_LOOP_NORMAL);
            channel->setLoopCount(-1);
        }
        channel->setVolume(1.0f);
        FMOD_VECTOR audioPosition = GetFMODVector(position);
        channel->set3DAttributes(&audioPosition, nullptr);
        channel->setPaused(false);
        // UpdateChannelPosition(channel, position);
    }
}

void AudioManager::UpdateChannelPosition(FMOD::Channel* channel, const glm::vec3& position) {
    if (!channel) return;

    FMOD_VECTOR fmodPosition = GetFMODVector(position);
    channel->set3DAttributes(&fmodPosition, nullptr);
}

FMOD_VECTOR AudioManager::GetFMODVector(const glm::vec3& position) {
    return { position.x, position.y, position.z };
}

void AudioManager::Update(float deltaTime) {
    fmodSystem->update();
    FMOD_VECTOR listenerPosition = {0.0f, 0.0f, 0.0f}; // Listener at origin
    fmodSystem->set3DListenerAttributes(0, &listenerPosition, nullptr, nullptr, nullptr);
}

FMOD::System* AudioManager::GetSystem() {
    return fmodSystem;
}