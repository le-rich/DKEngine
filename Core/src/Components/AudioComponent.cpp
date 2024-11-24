
#include "Components/AudioComponent.h"
#include "Managers/AssetManager.h"
#include "Managers/AudioManager.h"
#include "Entity.h"


AudioComponent::AudioComponent(Entity* mEntity, bool playOnStart, bool isLooping)
    : Component(mEntity), sound(nullptr), channel(nullptr), playOnStart(playOnStart), isLooping(isLooping)
{
    if (playOnStart)
        Play();
}

AudioComponent::~AudioComponent() {
    Stop();
    if (sound) {
        sound->release();
    }
    delete[] audioFilePath;
}


void AudioComponent::AddAudioSource(const char* filePath) {
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
        AddAudioSource(audioFilePath);
    }
    
    if (sound) {
        
        audioManager->GetSystem()->playSound(sound, nullptr, playOnStart, &channel);
        if (channel) {
            if (isLooping)
            {
                sound->setMode(FMOD_LOOP_NORMAL);
                channel->setLoopCount(-1);
            }
            
            channel->setVolume(1.0f);
            const FMOD_VECTOR audioPosition = GetFMODVector3(mEntity->transform->getLocalPosition());
            channel->set3DAttributes(&audioPosition, nullptr);
            channel->setPaused(false); // Start playback
            playOnStart = true;
        }
    } else {
        std::cerr << "AudioManager not available in manager!" << std::endl;
    }
}

void AudioComponent::Stop() {
    if (channel) {
        channel->stop();
        channel = nullptr;
        playOnStart = false;
    }
}

void AudioComponent::Update() {
    if (channel && playOnStart) {
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
    return playOnStart;
}

Component* AudioComponent::clone() const {
    return new AudioComponent(*this);
}