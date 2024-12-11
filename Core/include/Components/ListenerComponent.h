#ifndef LSTNCOMP_H
#define LSTNCOMP_H

#include "Component.h"
#include "Managers/FMODManager.h"

#include <fmod.hpp>
#include <glm.hpp>

class ListenerComponent : public Component {
public:
    ListenerComponent(Entity* mEntity, int listener);
    ~ListenerComponent();

    void UpdatePosition(float deltaTime);
    Component* clone() const override;
private:
    glm::vec3 previousPosition;
    int listener;
    float updateInterval = 0.02f; // Update every 20ms
    float accumulatedTime = 0.0f;
};
#endif