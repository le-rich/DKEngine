#include "AudioComponent.h"
#include "AudioManager.h"
#include <iostream>
#include "Entity.h"

AudioComponent::AudioComponent(Entity* entity, const char* filePath)
    : Component(entity), sound(nullptr), channel(nullptr), isPlaying(false), audioFilePath(nullptr)
{
    componentType = ComponentType::Audio;  // Adjust component type
    LoadSound(filePath);  // Load the sound file
}


AudioComponent::~AudioComponent()
{
    if (audioFilePath) {
        delete[] audioFilePath;  // Free the memory if it was allocated
    }
    if (sound) {
        sound->release();  // Release FMOD sound object
    }
}

void AudioComponent::LoadSound(const char* filePath)
{
    // If a file path was previously assigned, free it
    if (audioFilePath) {
        delete[] audioFilePath;
    }

    // Allocate memory for the new file path and copy it
    audioFilePath = new char[strlen(filePath) + 1];
    strcpy(audioFilePath, filePath);

    if (sound) {
        sound->release();  // Release any previously loaded sound
    }

    FMOD::System* system = AudioManager::getInstance()->getSystem();
    system->createSound(filePath, FMOD_DEFAULT, nullptr, &sound);
}

void AudioComponent::PlaySound()
{
    if (sound) {
        FMOD::System* system = AudioManager::getInstance()->getSystem();
        system->playSound(sound, nullptr, false, &channel);
        isPlaying = true;
    }
}

void AudioComponent::StopSound()
{
    if (channel) {
        channel->stop();
        isPlaying = false;
    }
}

void AudioComponent::Update()
{
    if (channel) {
        channel->isPlaying(&isPlaying);  // Update isPlaying status
    }
}

Component* AudioComponent::clone() const
{
    return new AudioComponent(entity, audioFilePath);  // Clone the component
}

bool AudioComponent::IsPlaying() const
{
    return isPlaying;  // Return whether the sound is playing
}
