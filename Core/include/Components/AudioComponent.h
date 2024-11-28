#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H

#include "Managers/AudioManager.h"
#include "Entity.h"
#include <string>

class AudioComponent : public Component {
public:
    AudioComponent(Entity* entity, const std::string& audioFilePath);
    ~AudioComponent();

    void Play();
    void Stop();
    void Update();
    void SetLooping(bool looping);
    void PlayOnAwake(bool play);
    bool IsPlaying() const;
    

    Component* clone() const override;

private:
    void LoadAudio();

    AudioManager* audioManager;
    FMOD::Sound* sound;
    FMOD::Channel* channel;
    std::string filePath;
    bool playOnAwake;
    bool isLooping;
};

#endif // AUDIOCOMPONENT_H