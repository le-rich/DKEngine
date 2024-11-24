#pragma once

#include "Component.h"
#include "FMOD/inc/fmod.hpp"
#include "Managers/AudioManager.h"

class AudioComponent : public Component
{
public:
    AudioComponent(Entity* entity);

    virtual ~AudioComponent();

    void LoadSound(const char* filePath);
    void Play();
    void Stop(); 
    void Update();
    void SetPosition(const FMOD_VECTOR& position);
    FMOD_VECTOR GetFMODVector3(glm::vec3 position);

    Component* clone() const override;

    bool IsPlaying() const;

private:
    char* audioFilePath;
    FMOD::Sound* sound;
    FMOD::Channel* channel;
    bool isPlaying;
    bool isLooping;
    AudioManager* audioManager;  // Reference to the audio manager
};
