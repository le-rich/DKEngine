#ifndef LSTNCOMP_H
#define LSTNCMOP_H

#include <Component.h>
#include <Managers/AudioManager.h>

#include <fmod.hpp>
#include <glm.hpp>

class ListenerComponent : public Component {
public:
	static ListenerComponent* getInstance(Entity* mEntity, AudioManager* audioManager);

	void UpdatePosition(float deltaTime);
private:
	Component* clone() const;
	ListenerComponent(Entity* mEntity, AudioManager* audioManager);
	~ListenerComponent();
	ListenerComponent(const ListenerComponent& lc) = delete;
	ListenerComponent& operator= (const ListenerComponent& lc) = delete;

	AudioManager* am = nullptr;
	glm::vec3 previousPosition;
	static ListenerComponent* pInstance;
    static std::mutex mutex_;
};
#endif