#ifndef FMODMANAGER_H
#define FMODMANAGER_H

#include "System.h"

#include <glm/vec3.hpp>
#include <fmod.hpp>

#include <unordered_map>
#include <string>

class FMODManager {
public:
    ~FMODManager();

    static FMODManager* GetInstance() {
        static FMODManager* pInstance = new FMODManager();
        return pInstance;
    }

    FMOD::Sound* LoadAudio(const std::string& filePath);
    static FMOD_VECTOR GetFMODVector(const glm::vec3& position);
    FMOD::System* GetFMODSystem();
private:
    FMODManager();
    FMOD::System* fmodSystem;
    std::unordered_map<std::string, FMOD::Sound*> soundCache;
    FMODManager(const FMODManager& lc) = delete;
    FMODManager& operator= (const FMODManager& lc) = delete;
};

static FMODManager* pInstance = FMODManager::GetInstance();


#endif // FMODMANAGER_H