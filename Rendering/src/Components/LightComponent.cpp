#include "Components/LightComponent.h"
#include "Entity.h"

#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>

LightComponent::LightComponent(Entity* pEntity) : Component(pEntity)
{
    this->componentType = ComponentType::Light;
}

LightComponent::LightComponent(Entity* pEntity, LightParams params) : Component(pEntity), mColor(params.color), mIntensity(params.intensity),
mConstant(params.constant), mLinear(params.linear), mQuadratic(params.quadratic), mCutoff(params.cutoff), mOuterCutoff(params.outercutoff), mCreatesShadows(params.createsShadows), mType(params.type)
{
    this->componentType = ComponentType::Light;
    if (mCreatesShadows)
    {
        glGenFramebuffers(1, &mShadowFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, mShadowFrameBuffer);

        // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
        glGenTextures(1, &mDepthTexture);
        glBindTexture(GL_TEXTURE_2D, mDepthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, mShadowMapSize, mShadowMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mDepthTexture, 0);

        glDrawBuffer(GL_NONE); // No color buffer is drawn to.

        // Always check that our framebuffer is ok
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "ERROR: Shadow map not created correctly" << std::endl;
    }
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
    result[3][1] = glm::cos(glm::radians(mCutoff));
    result[3][2] = glm::cos(glm::radians(mOuterCutoff));

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
    mCreatesShadows = pLightParams.createsShadows;
    mType = pLightParams.type;
}

void LightComponent::BindShadowFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mShadowFrameBuffer);
    glViewport(0, 0, mShadowMapSize, mShadowMapSize);
}

void LightComponent::BindShadowMap()
{
    glBindTexture(GL_TEXTURE_2D, mDepthTexture);
}

bool LightComponent::CheckIfPointExistsInFrustum(glm::vec3 lightPos, glm::mat4 lightTransformMatrix, glm::vec3 worldPoint)
{
    auto farDist = mFarClip ;
    auto nearDist = mNearClip ;

    auto posRel2lightera = worldPoint - lightPos;

    auto lightForward = -glm::normalize(glm::vec3(lightTransformMatrix[0][2], lightTransformMatrix[1][2], lightTransformMatrix[2][2]));
    auto zProjection = glm::dot(posRel2lightera, lightForward);
    if (zProjection > farDist || zProjection < nearDist)
    {
        return false;
    }

    auto tang = tan(glm::radians(90.f) * 0.5);
    auto lightUp = glm::normalize(glm::vec3(lightTransformMatrix[0][1], lightTransformMatrix[1][1], lightTransformMatrix[2][1]));
    auto yProjection = glm::dot(posRel2lightera, lightUp);
    auto minAngle = zProjection * tang;

    if (yProjection > minAngle || yProjection < -minAngle)
    {
        return false;
    }

    auto lightRight = glm::normalize(glm::vec3(lightTransformMatrix[0][0], lightTransformMatrix[1][0], lightTransformMatrix[2][0]));
    auto xProjection = glm::dot(posRel2lightera, lightRight);
    minAngle *= (float)mShadowMapSize / (float)mShadowMapSize;

    if (xProjection > minAngle || xProjection < -minAngle)
    {
        return false;
    }
    return true;
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