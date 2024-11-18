#pragma once

#include "Component.h"  // Include the base class Component
#include "FMOD/inc/fmod.hpp"  // Include FMOD for sound management
#include <string>

class AudioComponent : public Component
{
public:
    AudioComponent(Entity* mEntity);
    
    virtual ~AudioComponent();

    AudioComponent(Entity* mEntity, const std::string& filePath);


    void LoadSound(const std::string& filePath);

    void PlaySound();

    void StopSound();

    void Update();

    Component* clone() const override;

    bool IsPlaying() const;

private:
    std::string audioFilePath;  // File path to the audio file
    FMOD::Sound* sound;         // FMOD sound object
    FMOD::Channel* channel;     // FMOD channel for playback control
    bool isPlaying;             // Whether the sound is currently playing
};
