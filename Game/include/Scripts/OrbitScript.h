#pragma once

#include "Script.h"

#include "Components/TransformComponent.h"

class OrbitScript : public Script 
{
public:
    OrbitScript(Entity* mEntity);
    ~OrbitScript();  

    void Update(float deltaTime) override;

    void setOrbitTarget(TransformComponent* transform) { m_OrbitTarget = transform; }

    std::unique_ptr<Script> clone() const override {
        return std::make_unique<OrbitScript>(*this);
    }

private:
    TransformComponent* m_OrbitTarget = nullptr;

    float m_Radius = 2;
    float m_Speed = 1.5f;

    float currentAngle = 0;
};
