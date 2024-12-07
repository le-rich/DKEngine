#pragma once

#include "Component.h"
#include "Components/TransformComponent.h"

//#include <stdint.h>
#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <unordered_map>

enum LightType
{
    AmbientLight = 0,
    PointLight,
    DirectionalLight,
    SpotLight,
    AreaLight // Needs additional information that does not currently fit in generated matrix (reuse cutoff variables?)
};

std::unordered_map<std::string, LightType> const LightTypeMap = {
        {"AmbientLight", LightType::AmbientLight},
        {"PointLight", LightType::PointLight},
        {"DirectionalLight", LightType::DirectionalLight},
        {"SpotLight", LightType::SpotLight},
        {"AreaLight", LightType::AreaLight},
};

struct LightParams
{
    glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    float intensity = 1.f;
    float constant = 0.0f;
    float linear = 0.0f;
    float quadratic = 1.0f;
    float cutoff = 12.0f;
    float outercutoff = 15.0f;
    bool createsShadows = false;
    LightType type = AmbientLight;
};

static const float MAX_SHADOW_SIZE = 4096;

class LightComponent : public Component
{
public:
    LightComponent(Entity* pEntity);
    LightComponent(Entity* pEntity, LightParams params);
    LightComponent(const LightComponent& other);

    // Return single 4x4 matrix representing all LightComponent information
    glm::mat4 GenerateMatrix(TransformComponent* pTransform);

    inline glm::vec4 GetColor() const { return mColor; }
    inline float GetIntensity() const { return mIntensity; }
    inline float GetConstant() const { return mConstant; }
    inline float GetLinear() const { return mLinear; }
    inline float GetQuadratic() const { return mQuadratic; }
    inline float GetCutoff() const { return mCutoff; }
    inline float GetOuterCutoff() const { return mOuterCutoff; }
    inline bool GetCreatesShadows() const { return mCreatesShadows; }
    inline LightType GetType() const { return mType; }

    inline GLuint GetShadowMapID() const { return mDepthTexture; }
    inline glm::mat4 GetOrthoMatrix() const { return  glm::ortho<float>(-mOrthoBoundry, mOrthoBoundry, -mOrthoBoundry, mOrthoBoundry, mNearClip, mFarClip); }
    inline glm::mat4 GetPerspectiveMatrix() const { return  glm::perspective(glm::radians(mPerspectiveFOV), (float)mShadowMapSize / (float)mShadowMapSize, mNearClip, mFarClip); }

    inline void SetColor(glm::vec4 pColor) { mColor = pColor; }
    inline void SetIntensity(float pIntensity) { mIntensity = pIntensity; }
    inline void SetConstant(float pConstant) { mConstant = pConstant; }
    inline void SetLinear(float pLinear) { mLinear = pLinear; }
    inline void SetQuadratic(float pQuadratic) { mQuadratic = pQuadratic; }
    inline void SetCutoff(float pCutoff) { mCutoff = pCutoff; }
    inline void SetOuterCutoff(float pOuterCutoff) { mOuterCutoff = pOuterCutoff; }
    inline void SetType(LightType pType) { mType = pType; }

    void SetParams(LightParams pParams);
    void BindShadowFrameBuffer();
    void BindShadowMap();

    virtual Component* clone() const override;


private:

    glm::vec4 mColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    float mIntensity = 1.f;
    float mConstant = 0.0f;
    float mLinear = 0.0f;
    float mQuadratic = 1.0f;
    float mCutoff = 12.0f;
    float mOuterCutoff = 15.0f;
    bool mCreatesShadows = false;
    LightType mType = AmbientLight;

    float mShadowMapSize = 4096;
    float mNearClip = 1.f;
    float mFarClip = 7.5;
    float mOrthoBoundry = 10;
    float mPerspectiveFOV = 70.f;

    GLuint mShadowFrameBuffer = 0;
    GLuint mDepthTexture = 0;
};

