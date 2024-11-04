#pragma once

#include "Script.h"

#include "Components/Transform.h"

class OrbitScript : public Script 
{
public:
    OrbitScript(Entity* mEntity);
    ~OrbitScript();  

    void Update(float deltaTime) override;

    void setOrbitTarget(Transform* transform) { m_OrbitTarget = transform; }

private:
    Transform* m_OrbitTarget = nullptr;

    float m_Radius = 2;
    float m_Speed = 1.5f;

    float currentAngle = 0;
};
