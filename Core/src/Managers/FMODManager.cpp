#include "Managers/FMODManager.h"




FMODManager::FMODManager() {
    FMOD::System_Create(&fmodSystem);
    fmodSystem->init(512, 0, nullptr);
}

FMODManager::~FMODManager() {
    for (auto& pair : soundCache) {
        pair.second->release();
    }
    fmodSystem->release();
}

FMOD::Sound* FMODManager::LoadAudio(const std::string& filePath) {
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

FMOD_VECTOR FMODManager::GetFMODVector(const glm::vec3& position) {
    return { position.x, position.y, position.z };
}

FMOD::System* FMODManager::GetFMODSystem() {
    return fmodSystem;
}
