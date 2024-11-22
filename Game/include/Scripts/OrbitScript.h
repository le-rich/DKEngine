#pragma once

#include "Script.h"

#include "Components/TransformComponent.h"

struct OrbitScriptParams : ScriptParams
{
    TransformComponent* m_OrbitTarget = nullptr;
    TransformComponent* m_OrbitTarget2 = nullptr;

    float m_Radius = 2;
    float m_Speed = 1.5f;
    float currentAngle = 0;
    
};

class OrbitScript : public Script
{
public:
    OrbitScript(Entity* mEntity);
    ~OrbitScript();

    void Update(float deltaTime) override;
    void SetParameters(ScriptParams* pScriptParameters) override;

    void setOrbitTarget(TransformComponent* transform) { mParams.m_OrbitTarget = transform; }

    std::unique_ptr<Script> clone() const override
    {
        return std::make_unique<OrbitScript>(*this);
    }

    void swapOrbitTargets();
    TransformComponent* getCurrentTarget();

private:
    OrbitScriptParams mParams{};
};
