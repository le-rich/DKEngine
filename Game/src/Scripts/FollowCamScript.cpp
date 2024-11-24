#include "Scripts/FollowCamScript.h"

#include "Components/TransformComponent.h"

FollowCamScript::FollowCamScript(Entity* mEntity) : Script(mEntity)
{

}

FollowCamScript::~FollowCamScript() {}

bool follow = false;
void FollowCamScript::Update(float deltaTime)
{
    TransformComponent* transform = this->entity->transform;
    if (!transform || !mParams.m_FollowTarget) {
        return;
    }
    
    if (!follow) {
        transform->setLocalPosition(mParams.m_FollowTarget->getLocalPosition() + glm::vec3(0.0f, 1.5f, -4.25f));
        glm::quat q = mParams.m_FollowTarget->getLocalOrientation() * glm::quat(0, 0, -1, 0);
        transform->setLocalOrientation(q);
        follow = true;
    }
}

void FollowCamScript::SetParameters(ScriptParams* pScriptParameters)
{
    mParams = *static_cast<FollowCamScriptParams*>(pScriptParameters);
}
