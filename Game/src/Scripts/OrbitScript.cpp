#include "Scripts/OrbitScript.h"
#include "Components/TransformComponent.h"

OrbitScript::OrbitScript(Entity *mEntity) : Script(mEntity)
{
   
}

OrbitScript::~OrbitScript() {}

void OrbitScript::Update(float deltaTime)
{
    TransformComponent* transform = this->entity->transform;
    if (!transform || !mParams.m_OrbitTarget) {
        return;
    }
    
    mParams.currentAngle += mParams.m_Speed * deltaTime;

    float x = mParams.m_OrbitTarget->getWorldPosition().x + mParams.m_Radius * cos(mParams.currentAngle);
    float z = mParams.m_OrbitTarget->getWorldPosition().z + mParams.m_Radius * sin(mParams.currentAngle);

    glm::vec3 newPosition = glm::vec3(x, transform->getWorldPosition().y, z);
    glm::vec4 newPosition4 = glm::vec4(newPosition, 1.0f);
    transform->setWorldPosition(newPosition4);
    
    glm::vec3 direction = glm::normalize(mParams.m_OrbitTarget->getWorldPosition() - newPosition);
    glm::quat newRotation = glm::quatLookAt(direction, glm::vec3(0, 1, 0));
    transform->setWorldOrientation(newRotation);
}

void OrbitScript::SetParameters(ScriptParams* pScriptParameters)
{
    mParams = *static_cast<OrbitScriptParams*>(pScriptParameters);
}

void OrbitScript::swapOrbitTargets() {
    TransformComponent* temp = mParams.m_OrbitTarget;
    mParams.m_OrbitTarget = mParams.m_OrbitTarget2;
    mParams.m_OrbitTarget2 = temp;
}

TransformComponent* OrbitScript::getCurrentTarget() {
    return mParams.m_OrbitTarget;
}
