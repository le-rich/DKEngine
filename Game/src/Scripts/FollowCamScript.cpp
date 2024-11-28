#include "Scripts/FollowCamScript.h"

#include "Components/TransformComponent.h"

FollowCamScript::FollowCamScript(Entity* mEntity) : Script(mEntity)
{

}

FollowCamScript::~FollowCamScript() {}

bool follow = true;
void FollowCamScript::Update(float deltaTime)
{
    TransformComponent* transform = this->entity->transform;
    if (!transform || !mParams.m_FollowTarget) {
        return;
    }

    glm::vec3 cameraOffset = mParams.m_FollowTarget->getBack() * mParams.m_Distance;
    cameraOffset.y = mParams.m_Height;
    glm::vec3 targetPosition = mParams.m_FollowTarget->getLocalPosition() + cameraOffset;

    glm::vec3 currentPosition = transform->getLocalPosition();
    glm::vec3 newPosition = glm::mix(currentPosition, targetPosition, mParams.m_Speed * deltaTime); // this is just GLM's lerp from what i can tell
    transform->setLocalPosition(newPosition);

    // -1 quaternion rotation appended bc car points positive Z dir, opengl cam -Z dir.
    glm::quat targetOrientation = mParams.m_FollowTarget->getLocalOrientation() * glm::quat(0, 0, 1, 0); 
    glm::quat currentOrientation = transform->getLocalOrientation();
    glm::quat newOrientation = glm::slerp(currentOrientation, targetOrientation, mParams.m_Speed * deltaTime);
    transform->setLocalOrientation(newOrientation);
}

void FollowCamScript::SetParameters(ScriptParams* pScriptParameters)
{
    mParams = *static_cast<FollowCamScriptParams*>(pScriptParameters);
}
