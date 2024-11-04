#include "Scripts/OrbitScript.h"

#include "Components/Transform.h"

OrbitScript::OrbitScript(Entity *mEntity) : Script(mEntity)
{

}

OrbitScript::~OrbitScript() {}

void OrbitScript::Update(float deltaTime)
{
    Transform* transform = this->entity->transform;
    if (!transform || !m_OrbitTarget) {
        return;
    }
    
    currentAngle += m_Speed * deltaTime;

    float x = m_OrbitTarget->getWorldPosition().x + m_Radius * cos(currentAngle);
    float z = m_OrbitTarget->getWorldPosition().z + m_Radius * sin(currentAngle);

    glm::vec3 newPosition = glm::vec3(x, transform->getWorldPosition().y, z);
    glm::vec4 newPosition4 = glm::vec4(newPosition, 1.0f);
    transform->setWorldPosition(newPosition4);
    
    glm::vec3 direction = glm::normalize(m_OrbitTarget->getWorldPosition() - newPosition);
    glm::quat newRotation = glm::quatLookAt(direction, glm::vec3(0, 1, 0));
    transform->setWorldOrientation(newRotation);
}
