#include "Entity.h"
#include "Components/ListenerComponent.h"


ListenerComponent::ListenerComponent(Entity* mEntity, int listener) : Component(mEntity) {
    ListenerComponent::previousPosition = entity->transform->getWorldPosition();
    ListenerComponent::listener = listener;
    this->componentType = ComponentType::Listener;
}

void ListenerComponent::UpdatePosition(float deltaTime) {
    accumulatedTime += deltaTime;

    if (accumulatedTime >= updateInterval) {
        FMODManager* fmodMan = FMODManager::GetInstance();
        glm::vec3 posGlm = entity->transform->getWorldPosition();
        FMOD_VECTOR pos = FMODManager::GetFMODVector(posGlm);
        FMOD_VECTOR vel = FMODManager::GetFMODVector((posGlm - previousPosition) / accumulatedTime);
        FMOD_VECTOR forward = FMODManager::GetFMODVector(entity->transform->getForward());
        FMOD_VECTOR up = FMODManager::GetFMODVector(entity->transform->getUp());
        fmodMan->GetFMODSystem()->set3DListenerAttributes(0, &pos, &vel, &forward, &up);
        previousPosition = posGlm;
        accumulatedTime = 0.0f;
    }
}

ListenerComponent::~ListenerComponent() = default;

Component* ListenerComponent::clone() const {
    return new ListenerComponent(*this);
};