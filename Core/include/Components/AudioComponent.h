#ifndef AUDIOCOMP_H
#define AUDIOCOMP_H

#include "Component.h"
#include "Managers/FMODManager.h"

#include <fmod.hpp>
#include <glm.hpp>

class AudioComponent : public Component {
public:
    AudioComponent(Entity* mEntity);
    AudioComponent(Entity* mEntity, FMOD::ChannelGroup* channelGroup);
    AudioComponent(Entity* mEntity, FMOD::Sound* sound);
    AudioComponent(Entity* mEntity, FMOD::ChannelGroup* channelGroup, FMOD::Sound* sound);

    ~AudioComponent();

    void PlaySound(FMOD::Sound* sound, bool isLooping, bool isPaused);

    void UpdateAttributes(float deltaTime);
    
    AudioComponent(const AudioComponent& other);
    Component* clone() const override;

    FMOD::Channel* GetChannel();
private:
    FMOD::Channel* channel = nullptr;
    FMOD::ChannelGroup* group = nullptr;
    glm::vec3 previousPosition;
};

#endif