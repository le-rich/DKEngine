#include "Objects/Light.h"

Light::Light(glm::vec3 pWorldPos, glm::vec3 pWorldRotationEuler, LightType pType) : mWorldPos(pWorldPos), mType(pType)
{
    SetWorldRotationFromEuler(pWorldRotationEuler);
}

// TODO: fill in with transform format
Light::Light(glm::mat4 transform, LightType pType) : mType(pType)
{}



glm::mat4 Light::GenerateMatrix()
{

    glm::mat4 result;

    result[0][0] = mWorldPos.x;
    result[0][1] = mWorldPos.y;
    result[0][2] = mWorldPos.z;

    glm::vec3 worldForward = mWorldRotation * glm::vec3(0.f, 0.f, 1.f);

    result[1][0] = worldForward.x;
    result[1][1] = worldForward.y;
    result[1][2] = worldForward.z;

    // Compress color into single float value
    result[2][0] = static_cast<float>(static_cast<uint32_t>(
        (static_cast<uint8_t>(mColor[0] * 255.0f) << 24) |
        (static_cast<uint8_t>(mColor[1] * 255.0f) << 16) |
        (static_cast<uint8_t>(mColor[2] * 255.0f) <<  8) |
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

void Light::SetWorldRotationFromEuler(glm::vec3 pWorldRoationEuler)
{
    glm::mat4 rotationMatrix{ 1.0f };
    glm::mat4 rotationX{ 1.0f };
    //rotationX = glm::rotate(rotationX, glm::radians(pWorldRotoationEuler.x), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 rotationY{ 1.0f };
    //rotationY = glm::rotate(rotationY, glm::radians(pWorldRotoationEuler.y), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 rotationZ{ 1.0f };
    //rotationZ = glm::rotate(rotationZ, glm::radians(pWorldRotoationEuler.z), glm::vec3(0.0f, 0.0f, 1.0f));

    rotationMatrix = rotationZ * rotationY * rotationX;
    //mWorldRotation = glm::quat_cast(rotationMatrix);
}
