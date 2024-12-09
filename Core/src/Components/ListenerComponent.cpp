

#include <Entity.h>
#include <Components/ListenerComponent.h>
#include <Managers/AudioManager.h>

#include <fmod.hpp>
#include <glm.hpp>

ListenerComponent* ListenerComponent::pInstance = nullptr;
std::mutex ListenerComponent::mutex_;

ListenerComponent* ListenerComponent::getInstance(Entity* mEntity, AudioManager* audioManager) {
    if (pInstance == nullptr) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (pInstance == nullptr) {
            ListenerComponent* temp = new ListenerComponent(mEntity, audioManager);
            pInstance = temp;
        }
    }
    return pInstance;
}

ListenerComponent::ListenerComponent(Entity* mEntity, AudioManager* audioManager) : Component(mEntity) {
	ListenerComponent::previousPosition = entity->transform->getWorldPosition();
	ListenerComponent::am = audioManager;
}

void ListenerComponent::UpdatePosition(float deltaTime) {
	glm::vec3 posGlm = entity->transform->getWorldPosition();
	FMOD_VECTOR pos = AudioManager::GetFMODVector(posGlm);
	FMOD_VECTOR vel = AudioManager::GetFMODVector(posGlm - previousPosition);
	FMOD_VECTOR forward = AudioManager::GetFMODVector(entity->transform->getForward());
	FMOD_VECTOR up = AudioManager::GetFMODVector(entity->transform->getUp());
	am->GetSystem()->set3DListenerAttributes(0, &pos, &vel, &forward, &up);
	previousPosition = posGlm;
}

ListenerComponent::~ListenerComponent() = default;

Component* ListenerComponent::clone() const {
	return nullptr;
};

