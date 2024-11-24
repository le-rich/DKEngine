#include "Components/LightComponent.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>

LightComponent::LightComponent(Entity* pEntity) : Component(pEntity)
{
    this->componentType = ComponentType::Light;
}

LightComponent::LightComponent(Entity* pEntity, LightParams params) : Component(pEntity), mColor(params.color), mIntensity(params.intensity), 
mConstant(params.constant), mLinear(params.linear), mQuadratic(params.quadratic), mCutoff(params.cutoff), mOuterCutoff(params.outercutoff), mType(params.type)
{
    this->componentType = ComponentType::Light;
}

glm::mat4 LightComponent::GenerateMatrix(TransformComponent* pTransform)
{
    glm::vec3 worldPos = pTransform->getWorldPosition();
    glm::quat worldRotation = pTransform->getWorldOrientation();
    glm::mat4 result;

    result[0][0] = worldPos.x;
    result[0][1] = worldPos.y;
    result[0][2] = worldPos.z;

    glm::vec3 worldForward = worldRotation * glm::vec3(0.f, 0.f, 1.f);

    result[1][0] = worldForward.x;
    result[1][1] = worldForward.y;
    result[1][2] = worldForward.z;

    // Compress color into single float value
    result[2][0] = static_cast<float>(static_cast<uint32_t>(
        (static_cast<uint8_t>(mColor[0] * 255.0f) << 24) |
        (static_cast<uint8_t>(mColor[1] * 255.0f) << 16) |
        (static_cast<uint8_t>(mColor[2] * 255.0f) << 8) |
        (static_cast<uint8_t>(mColor[3])
            )));

    result[3][0] = mType;
    result[3][1] = mCutoff;
    result[3][2] = mOuterCutoff;

    result[0][3] = mConstant;
    result[1][3] = mLinear;
    result[2][3] = mQuadratic;
    result[3][3] = mIntensity;

    return result;
}

void LightComponent::SetParams(LightParams pLightParams)
{
    mColor = pLightParams.color;
    mIntensity = pLightParams.intensity;
    mConstant = pLightParams.constant;
    mLinear = pLightParams.linear;
    mQuadratic = pLightParams.quadratic;
    mCutoff = pLightParams.cutoff;
    mOuterCutoff = pLightParams.outercutoff;
    mType = pLightParams.type;
}

Component* LightComponent::clone() const
{
    return new LightComponent(*this);
}

LightComponent::LightComponent(const LightComponent& other) : Component(other)
{
    mColor = other.mColor;
    mIntensity = other.mIntensity;
    mConstant = other.mConstant;
    mLinear = other.mLinear;
    mQuadratic = other.mQuadratic;
    mCutoff = other.mCutoff;
    mOuterCutoff = other.mOuterCutoff;
    mType = other.mType;
}