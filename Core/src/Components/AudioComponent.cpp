#include "Entity.h"
#include "Components/AudioComponent.h"
#include "Managers/FMODManager.h"


AudioComponent::AudioComponent(Entity* mEntity) : Component(mEntity) {
    AudioComponent::previousPosition = entity->transform->getWorldPosition();
    this->componentType = ComponentType::Audio;
};

AudioComponent::AudioComponent(Entity* mEntity, FMOD::ChannelGroup* channelGroup) : Component(mEntity) {
    AudioComponent::previousPosition = entity->transform->getWorldPosition();
    AudioComponent::group = channelGroup;
    this->componentType = ComponentType::Audio;
};

AudioComponent::AudioComponent(Entity* mEntity, FMOD::Sound* sound) : AudioComponent(mEntity) {
    FMOD_VECTOR pos = FMODManager::GetFMODVector(previousPosition);
    FMODManager::GetInstance()->GetFMODSystem()->playSound(sound, group, true, &channel);
    channel->set3DAttributes(&pos, nullptr);
};


AudioComponent::AudioComponent(Entity* mEntity, FMOD::ChannelGroup* channelGroup, FMOD::Sound* sound) : AudioComponent(mEntity, channelGroup) {
    FMOD_VECTOR pos = FMODManager::GetFMODVector(previousPosition);
    FMOD_RESULT result = FMODManager::GetInstance()->GetFMODSystem()->playSound(sound, group, true, &channel);
    if (result == FMOD_OK) {
        channel->setChannelGroup(group);
        channel->set3DAttributes(&pos, nullptr);
    }
};

AudioComponent::~AudioComponent() {};

void AudioComponent::PlaySound(FMOD::Sound* sound, bool isLooping, bool isPaused) {
    if (!sound) { return; }

    if (isLooping) { sound->setMode(FMOD_LOOP_NORMAL); }

    FMOD_RESULT result = FMODManager::GetInstance()->GetFMODSystem()->playSound(sound, group, true, &channel);
    if (result == FMOD_OK && channel) {
        channel->setLoopCount(-1);
        FMOD_VECTOR pos = FMODManager::GetFMODVector(entity->transform->getWorldPosition());
        channel->set3DAttributes(&pos, nullptr);
        channel->setPaused(isPaused);
    }
};

void AudioComponent::UpdateAttributes(float deltaTime) {
    glm::vec3 currentPosition = entity->transform->getWorldPosition();
    FMOD_VECTOR vel = FMODManager::GetFMODVector((currentPosition - previousPosition) / deltaTime);
    FMOD_VECTOR pos = FMODManager::GetFMODVector(currentPosition);
    if (channel) {
        channel->set3DAttributes(&pos, &vel);
    }
    previousPosition = currentPosition;
};

FMOD::Channel* AudioComponent::GetChannel() {
    return channel;
}

AudioComponent::AudioComponent(const AudioComponent& other) : AudioComponent(other.entity, other.group) {} // Clone everyhing but channel, position will be updated.

Component* AudioComponent::clone() const {
    return new AudioComponent(*this);
};