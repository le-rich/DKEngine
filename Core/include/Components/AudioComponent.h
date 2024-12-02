#ifndef AUDIOCOMP_H
#define AUDIOCOMP_H

#include "Component.h"

#include <fmod.hpp>
#include "Managers/AudioManager.h"
#include <glm.hpp>

class AudioComponent : public Component {
public:
	AudioComponent(Entity* mEntity, AudioManager* audioManager);
	AudioComponent(Entity* mEntity, AudioManager* audioManager, FMOD::ChannelGroup* channelGroup);
	AudioComponent(Entity* mEntity, AudioManager* audioManager, FMOD::Sound* sound);
	AudioComponent(Entity* mEntity, AudioManager* audioManager, FMOD::ChannelGroup* channelGroup, FMOD::Sound* sound);

	~AudioComponent();

	void PlaySound(FMOD::Sound* sound, bool isLooping, bool isPaused);

	void UpdateAttributes(float deltaTime);

	AudioComponent(const AudioComponent& other);
	Component* clone() const override;

	FMOD::Channel* GetChannel();
private:
	AudioManager* am = nullptr;
	FMOD::Channel* channel = nullptr;
	FMOD::ChannelGroup* group = nullptr;
	glm::vec3 previousPosition;
};

#endif