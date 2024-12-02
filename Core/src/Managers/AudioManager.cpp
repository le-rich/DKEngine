﻿#include <Managers/AudioManager.h>
#include <Components/AudioComponent.h>
#include <Managers/EntityManager.h>

AudioManager::AudioManager() {
    FMOD::System_Create(&fmodSystem);
    listenerPosition = glm::vec3(0.0f, 0.0f, 0.0f);
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
    FMOD_VECTOR FMODListenerPosition = GetFMODVector(listenerPosition);

    std::vector<UUIDv4::UUID> audioEntitiesUUIDs = EntityManager::getInstance().findEntitiesByComponent(ComponentType::Audio);
    for (UUIDv4::UUID audioEntityUUID : audioEntitiesUUIDs) {
        Entity* audioEntity = EntityManager::getInstance().getEntity(audioEntityUUID);
        AudioComponent* audioComponent = dynamic_cast<AudioComponent*>(
            audioEntity->getComponent(ComponentType::Audio)
        );
        audioComponent->UpdateAttributes(deltaTime);

    }

    fmodSystem->set3DListenerAttributes(0, &FMODListenerPosition, nullptr, nullptr, nullptr);

    fmodSystem->update();
}

FMOD::System* AudioManager::GetSystem() {
    return fmodSystem;
}

void AudioManager::updateListenerPosition(const Entity* listenerParent) {

    glm::vec3 posGlm = listenerParent->transform->getWorldPosition();
    FMOD_VECTOR pos = AudioManager::GetFMODVector(posGlm);
    FMOD_VECTOR vel = AudioManager::GetFMODVector(posGlm - listenerPosition);
    FMOD_VECTOR forward = AudioManager::GetFMODVector(listenerParent->transform->getForward());
    FMOD_VECTOR up = AudioManager::GetFMODVector(listenerParent->transform->getUp());

    fmodSystem->set3DListenerAttributes(0, &pos, &vel, &forward, &up);
}

