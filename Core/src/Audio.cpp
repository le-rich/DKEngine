#include "Audio.h"
#include "Components/AudioComponent.h"
#include "Managers/EntityManager.h"
#include "Components/ListenerComponent.h"

Audio::Audio() {
    FMODManager::GetInstance();
}

void Audio::Initialize()
{
    System::Initialize();
}

Audio::~Audio() {}

void Audio::FixedUpdate() {} // No use right now, just used for virtual override.

void Audio::Update(float deltaTime) {
    std::vector<UUIDv4::UUID> audioEntitiesUUIDs = EntityManager::getInstance().findEntitiesByComponent(ComponentType::Audio);
    for (UUIDv4::UUID audioEntityUUID : audioEntitiesUUIDs) {
        Entity* audioEntity = EntityManager::getInstance().getEntity(audioEntityUUID);
        AudioComponent* audioComponent = dynamic_cast<AudioComponent*>(
            audioEntity->getComponent(ComponentType::Audio)
            );
        audioComponent->UpdateAttributes(deltaTime);

    }
    std::vector<UUIDv4::UUID> listenerEntitiesUUIDs = EntityManager::getInstance().findEntitiesByComponent(ComponentType::Listener);
    for (UUIDv4::UUID listenerEntityUUID : listenerEntitiesUUIDs) {
        Entity* listenerEntity = EntityManager::getInstance().getEntity(listenerEntityUUID);
        ListenerComponent* listenerComponent = dynamic_cast<ListenerComponent*>(
            listenerEntity->getComponent(ComponentType::Listener)
            );
        listenerComponent->UpdatePosition(deltaTime);

    }

    FMODManager::GetInstance()->GetFMODSystem()->update();
}

FMOD::System* Audio::GetSystem() {
    return FMODManager::GetInstance()->GetFMODSystem();
}
