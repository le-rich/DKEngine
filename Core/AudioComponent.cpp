#pragma once

#include "Entity.h"
#include "AudioComponent.h"
#include "Managers/AudioManager.h"


AudioComponent::AudioComponent(Entity* mEntity, AudioManager* audioManager) : Component(mEntity) {
	AudioComponent::previousPosition = entity->transform->getWorldPosition();
	AudioComponent::am = audioManager;
};

AudioComponent::AudioComponent(Entity* mEntity, AudioManager* audioManager, FMOD::ChannelGroup* channelGroup) : Component(mEntity) {
	AudioComponent::previousPosition = entity->transform->getWorldPosition();
	AudioComponent::am = audioManager;
	AudioComponent::group = channelGroup;
};

AudioComponent::AudioComponent(Entity* mEntity, AudioManager* audioManager, FMOD::Sound* sound) : AudioComponent(mEntity, audioManager){
	FMOD_VECTOR pos = AudioManager::GetFMODVector(previousPosition);
	// FMOD_VECTOR forward = AudioManager::GetFMODVector(entity->transform->getForward());
	// FMOD_VECTOR up = AudioManager::GetFMODVector(entity->transform->getUp());
	am->GetSystem()->playSound(sound, group, true, &channel);
	channel->set3DAttributes(&pos, nullptr);
};


AudioComponent::AudioComponent(Entity* mEntity, AudioManager* audioManager, FMOD::ChannelGroup* channelGroup, FMOD::Sound* sound) : AudioComponent(mEntity, audioManager, channelGroup) {
	FMOD_VECTOR pos = AudioManager::GetFMODVector(previousPosition);
	FMOD_VECTOR forward = AudioManager::GetFMODVector(entity->transform->getForward());
	FMOD_VECTOR up = AudioManager::GetFMODVector(entity->transform->getUp());
	FMOD_RESULT result = am->GetSystem()->playSound(sound, group, true, &channel);
	if (result == FMOD_OK)
		channel->setChannelGroup(group);
	channel->set3DAttributes(&pos, nullptr);
};

AudioComponent::~AudioComponent() {

};

void AudioComponent::PlaySound(FMOD::Sound* sound, bool isLooping, bool isPaused) {
	if (!sound) return;
	if (isLooping)
		sound->setMode(FMOD_LOOP_NORMAL);
	FMOD_RESULT result = am->GetSystem()->playSound(sound, group, true, &channel);
	if (result == FMOD_OK && channel) {
		channel->setLoopCount(-1);
		FMOD_VECTOR pos = AudioManager::GetFMODVector(entity->transform->getWorldPosition());
		channel->set3DAttributes(&pos, nullptr);
		channel->setPaused(isPaused);
	}
};

void AudioComponent::update(float deltaTime) {
	glm::vec3 currentPosition = entity->transform->getWorldPosition();
	FMOD_VECTOR vel = AudioManager::GetFMODVector((currentPosition - previousPosition) / deltaTime);
	FMOD_VECTOR pos = AudioManager::GetFMODVector(currentPosition);
	if (channel)
		channel->set3DAttributes(&pos, &vel);
};

FMOD::Channel* AudioComponent::GetChannel() {
	return channel;
}

Component* AudioComponent::clone() const {
	return new AudioComponent(*this);
};