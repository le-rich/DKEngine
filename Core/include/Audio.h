#ifndef AUDIOSYS_H
#define AUDIOSYS_H

#include "System.h"

#include <glm/vec3.hpp>
#include <fmod.hpp>

class Audio : public System {
public:
    Audio();
    ~Audio();

    const char* GetName() const override {
        return "Audio System";
    };

    void Initialize() override;
    FMOD::System* GetSystem();
    void Update(float deltaTime) override;
    void FixedUpdate() override;
private:
};

#endif // AUDIOSYS_H