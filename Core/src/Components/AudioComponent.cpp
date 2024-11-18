#pragma once

#include "Component.h"
#include "FMOD/inc/fmod.hpp" // Include FMOD for sound management
#include <string>

#include "AudioManager.h"
#include "System.h"

class AudioComponent : public Component
{
public:
    AudioComponent(Entity* mEntity) : Component(mEntity), sound(nullptr), channel(nullptr), isPlaying(false)
    {
        componentType = ComponentType::Audio;
    }

    AudioComponent(Entity* mEntity, const std::string& filePath)
        : Component(mEntity), sound(nullptr), channel(nullptr), isPlaying(false), audioFilePath(filePath)
    {
        componentType = ComponentType::Audio;  // Adjust to the appropriate type
        LoadSound(filePath);
    }

    virtual ~AudioComponent()
    {
        if (sound) {
            sound->release();
        }
    }

    void LoadSound(const std::string& filePath)
    {
        audioFilePath = filePath;
        if (sound) {
            sound->release();  // Release the previous sound if it exists
        }
        
        FMOD::System* system = AudioManager::getInstance()->getSystem();
        system->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &sound);
    }

    void PlaySound()
    {
        if (sound) {
            FMOD::System* system = AudioManager::getInstance()->getSystem();
            system->playSound(sound, nullptr, false, &channel);
            isPlaying = true;
        }
    }

    // Stop the sound if it is playing
    void StopSound()
    {
        if (channel) {
            channel->stop();
            isPlaying = false;
        }
    }

    void Update()
    {
        if (channel) {
            channel->isPlaying(&isPlaying); // Check if the sound is still playing
        }
    }

    Component* clone() const override
    {
        AudioComponent* newAudioComp = new AudioComponent(entity, audioFilePath);
        return newAudioComp;
    }

    // Getter for isPlaying
    bool IsPlaying() const
    {
        return isPlaying;
    }

private:
    std::string audioFilePath;  // File path to the sound file
    FMOD::Sound* sound;         // FMOD sound object
    FMOD::Channel* channel;     // FMOD channel to control playback
    bool isPlaying;             // Is the sound currently playing
};
