#pragma once

#include "Script.h"

#include "Components/TransformComponent.h"

struct FollowCamScriptParams : ScriptParams
{
    TransformComponent* m_FollowTarget = nullptr;

    float m_Distance = 3.0f;
    float m_Speed = 4.0f;
    float m_Height = 1.65f;
    float currentAngle = 0;
};

class FollowCamScript : public Script
{
public:
    FollowCamScript(Entity* mEntity);
    ~FollowCamScript();

    void Update(float deltaTime) override;
    void SetParameters(ScriptParams* pScriptParameters) override;

    void setFollowTarget(TransformComponent* transform) { mParams.m_FollowTarget = transform; }

    std::unique_ptr<Script> clone() const override
    {
        return std::make_unique<FollowCamScript>(*this);
    }

private:
    FollowCamScriptParams mParams{};
};
