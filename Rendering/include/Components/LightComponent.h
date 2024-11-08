#pragma once

//#include <stdint.h>
#include <glm.hpp>
#include <gtc/quaternion.hpp>

#include "Component.h"
#include "Components/TransformComponent.h"

enum LightType
{
    AmbientLight = 0,
    PointLight,
    DirectionalLight,
    SpotLight,
    AreaLight // Needs additional information that does not currently fit in generated matrix (reuse cutoff variables?)
};

class LightComponent : public Component
{
public:
    LightComponent(Entity* pEntity);
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
    inline float GetType() const { return mType; }

    inline void SetColor(glm::vec4 pColor) { mColor = pColor; }
    inline void SetIntensity(float pIntensity) { mIntensity = pIntensity; }
    inline void SetConstant(float pConstant) { mConstant = pConstant; }
    inline void SetLinear(float pLinear) { mLinear = pLinear; }
    inline void SetQuadratic(float pQuadratic) { mQuadratic = pQuadratic; }
    inline void SetCutoff(float pCutoff) { mCutoff = pCutoff; }
    inline void SetOuterCutoff(float pOuterCutoff) { mOuterCutoff = pOuterCutoff; }
    inline void SetType(LightType pType) { mType = pType; }

    Component* clone() const override;


private:
    glm::vec4 mColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    float mIntensity = 1.f;
    float mConstant = 0.0f;
    float mLinear = 0.0f;
    float mQuadratic = 1.0f;
    float mCutoff = 12.0f;
    float mOuterCutoff = 15.0f;
    LightType mType = AmbientLight;
};

