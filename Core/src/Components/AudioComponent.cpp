
#include "Components/AudioComponent.h"
#include "Managers/AssetManager.h"
#include "Managers/AudioManager.h"
#include "Entity.h"

AudioComponent::AudioComponent(Entity* entity, const std::string& audioFilePath)
    : Component(entity), sound(nullptr), channel(nullptr), filePath(audioFilePath), playOnAwake(false), isLooping(false) {
    audioManager = &AudioManager::GetInstance();
    LoadAudio();
    if (playOnAwake) {
        Play();
    }
}

AudioComponent::~AudioComponent() {
    Stop();
    if (sound) {
        sound->release();
    }
}

void AudioComponent::SetLooping(bool looping)
{
    isLooping = looping;
}

void AudioComponent::PlayOnAwake(bool play)
{
    playOnAwake = play;
}

void PlayOnAwake(bool playOnAwake);

void AudioComponent::LoadAudio() {
    if (!audioManager) {
        std::cerr << "AudioManager not available!" << std::endl;
        return;
    }

    sound = audioManager->LoadAudio(filePath);
    if (!sound) {
        std::cerr << "Failed to load audio: " << filePath << std::endl;
    }
}

void AudioComponent::Play() {
    if (!sound) {
        LoadAudio();
    }

    if (sound) {
        audioManager->PlaySound(sound, isLooping, channel, entity->transform->getLocalPosition());
        playOnAwake = true;
    }
}

void AudioComponent::Stop() {
    if (channel) {
        channel->stop();
        channel = nullptr;
        playOnAwake = false;
    }
}

void AudioComponent::Update() {
    if (channel) {
        audioManager->UpdateChannelPosition(channel, entity->transform->getLocalPosition());
    }
}

bool AudioComponent::IsPlaying() const {
    return playOnAwake;
}

Component* AudioComponent::clone() const {
    return new AudioComponent(*this);
}