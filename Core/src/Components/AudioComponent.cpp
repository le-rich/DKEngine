
#include "Components/AudioComponent.h"
#include "Managers/AssetManager.h"
#include "Managers/AudioManager.h"
#include "Entity.h"


AudioComponent::AudioComponent(Entity* mEntity, bool playOnStart, bool isLooping)
    : Component(mEntity), sound(nullptr), channel(nullptr), isPlaying(playOnStart), isLooping(isLooping)
{
    if (isPlaying)
        Play();
}

AudioComponent::~AudioComponent() {
    Stop();
    if (sound) {
        sound->release();
    }
    delete[] audioFilePath;
}


void AudioComponent::LoadSound(const char* filePath) {
    if (sound) {
        sound->release();
    }
    
    if (audioManager) {
        sound = AudioManager::GetInstance().LoadAudio(filePath);
    } else {
        std::cerr << "AudioManager not available in entity!" << std::endl;
    }
}


void AudioComponent::Play() {
    if (!sound) {
        LoadSound(audioFilePath);
    }
    if (sound) {
        
        audioManager->GetSystem()->playSound(sound, nullptr, isPlaying, &channel);
        if (channel) {
            const FMOD_VECTOR fmodPosition = GetFMODVector3(mEntity->transform->getLocalPosition());
            channel->set3DAttributes(&fmodPosition, nullptr);
            isPlaying = true;
        }
    } else {
        std::cerr << "AudioManager not available in entity!" << std::endl;
    }
}

void AudioComponent::Stop() {
    if (channel) {
        channel->stop();
        channel = nullptr;
        isPlaying = false;
    }
}

void AudioComponent::Update() {
    if (channel && isPlaying) {
        const FMOD_VECTOR fmodPosition = GetFMODVector3(mEntity->transform->getLocalPosition());
        channel->set3DAttributes(&fmodPosition, nullptr);
    }
}

FMOD_VECTOR AudioComponent::GetFMODVector3(glm::vec3 position)
{
    return { position.x, position.y, position.z };
}


void AudioComponent::SetPosition(const FMOD_VECTOR& position) {
    if (channel) {
        const FMOD_VECTOR fmodPosition = GetFMODVector3(mEntity->transform->getLocalPosition());
        channel->set3DAttributes(&fmodPosition, nullptr);
    }
}

bool AudioComponent::IsPlaying() const {
    return isPlaying;
}

Component* AudioComponent::clone() const {
    return new AudioComponent(*this);
}