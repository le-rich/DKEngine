#pragma once

#include "Component.h"
#include "FMOD/inc/fmod.hpp"
#include "Managers/AudioManager.h"

class AudioComponent : public Component
{
public:
    AudioComponent(Entity* entity, const char* audioFilePath);

    virtual ~AudioComponent();

    void LoadSound(const char* filePath);
    void Play();
    void Stop(); 
    void Update();
    void SetPosition(const FMOD_VECTOR& position);
    Component* clone() const override;

    bool IsPlaying() const;

private:
    char* audioFilePath;
    FMOD::Sound* sound;
    FMOD::Channel* channel;
    bool isPlaying;
    AudioManager* audioManager;  // Reference to the audio manager
};
