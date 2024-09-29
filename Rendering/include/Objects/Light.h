#pragma once

#include <stdint.h>

// Temporary definitions until glm library or similar is included
namespace glm
{
    typedef mat4;
    typedef vec3;
    typedef vec4;
    typedef quat;
};

enum LightType
{
    AmbientLight = 0,
    PointLight,
    DirectionalLight,
    SpotLight,
    AreaLight // Needs additional information that does not currently fit in generated matrix (reuse cutoff variables?)
};

class Light
{
public:
    Light(glm::vec3 pWorldPos, LightType pType = AmbientLight) : mWorldPos(pWorldPos), mType(pType) {}
    Light(glm::vec3 pWorldPos, glm::quat pWorldRotation, LightType pType = AmbientLight) : mWorldPos(pWorldPos), mWorldRotation(pWorldRotation), mType(pType) {}
    Light(glm::vec3 pWorldPos, glm::vec3 pWorldRotationEuler, LightType pType = AmbientLight);
    Light(glm::mat4 transform, LightType pType = AmbientLight);

    // Return single 4x4 matrix representing all light information
    glm::mat4 GenerateMatrix();
    void SetWorldRotationFromEuler(glm::vec3 pWorldRoationEuler);

    inline glm::vec3 GetWorldPos() const { return mWorldPos; }
    inline glm::quat GetWorldRotation() const { return mWorldRotation; }
    inline glm::vec4 GetColor() const { return mColor; }
    inline float GetIntensity() const { return mIntensity; }
    inline float GetConstant() const { return mConstant; }
    inline float GetLinear() const { return mLinear; }
    inline float GetQuadratic() const { return mQuadratic; }
    inline float GetCutoff() const { return mCutoff; }
    inline float GetOuterCutoff() const { return mOuterCutoff; }
    inline float GetType() const { return mType; }

    inline void SetWorldPos(glm::vec3 pWorldPos) { mWorldPos = pWorldPos; }
    inline void SetWorldRotation(glm::quat pWorldRotation) { mWorldRotation = pWorldRotation; }
    inline void SetColor(glm::vec3 pColor) { mColor = pColor; }
    inline void SetIntensity(float pIntensity) { mIntensity = pIntensity; }
    inline void SetConstant(float pConstant) { mConstant = pConstant; }
    inline void SetLinear(float pLinear) { mLinear = pLinear; }
    inline void SetQuadratic(float pQuadratic) { mQuadratic = pQuadratic; }
    inline void SetCutoff(float pCutoff) { mCutoff = pCutoff; }
    inline void SetOuterCutoff(float pOuterCutoff) { mOuterCutoff = pOuterCutoff; }
    inline void SetType(LightType pType) { mType = pType; }


private:
    // location in world
    glm::vec3 mWorldPos = glm::vec3(0.f, 0.f, 0.f);
    // direction camera is facing, calculated with Azimuth and Elevation
    glm::quat mWorldRotation = glm::quat(0.f, 0.f, 0.f, 0.f);

    glm::vec4 mColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    float mIntensity = 1.f;
    float mConstant = 0.0f;
    float mLinear = 0.0f;
    float mQuadratic = 1.0f;
    float mCutoff = 12.0f;
    float mOuterCutoff = 15.0f;
    LightType mType = AmbientLight;
};

