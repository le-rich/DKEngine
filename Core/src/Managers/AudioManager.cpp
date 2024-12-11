#include <Managers/AudioManager.h>
#include <Components/AudioComponent.h>
#include <Managers/EntityManager.h>
#include <Components/ListenerComponent.h>

AudioManager* AudioManager::pInstance;
std::mutex AudioManager::mutex_;

AudioManager* AudioManager::GetInstance() {
    if (pInstance == nullptr) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (pInstance == nullptr) {
            AudioManager* temp = new AudioManager();
            pInstance = temp;
        }
    }
    return pInstance;
}

AudioManager::AudioManager() {
    FMOD::System_Create(&fmodSystem);
}

void AudioManager::Initialize()
{
    fmodSystem->init(512, 0, nullptr);
}

AudioManager::~AudioManager() {
    for (auto& pair : soundCache) {
        pair.second->release();
    }
    fmodSystem->release();
}

FMOD::Sound* AudioManager::LoadAudio(const std::string& filePath) {
    auto it = soundCache.find(filePath);
    if (it != soundCache.end()) {
        return it->second;
    }

    FMOD::Sound* newSound;
    FMOD_RESULT result = fmodSystem->createSound(filePath.c_str(), FMOD_3D, nullptr, &newSound);
    if (result != FMOD_OK) {
        std::cerr << "Failed to load sound: " << filePath << std::endl;
        return nullptr;
    }

    soundCache[filePath] = newSound;
    return newSound;
}

void AudioManager::FixedUpdate() {} // No use right now, just used for virtual override.

FMOD_VECTOR AudioManager::GetFMODVector(const glm::vec3& position) {
    return { position.x, position.y, position.z };
}

void AudioManager::Update(float deltaTime) {
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
            listenerEntity->getComponent(ComponentType::Audio)
            );
        listenerComponent->UpdatePosition(deltaTime);

    }
    
    fmodSystem->update();
}

FMOD::System* AudioManager::GetSystem() {
    return fmodSystem;
}
