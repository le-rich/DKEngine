#include "Components/CameraComponent.h"

CameraComponent::CameraComponent(Entity* mEntity) : Component(mEntity)
{

}

CameraComponent::~CameraComponent()
{}

void CameraComponent::calculateViewMatrix(Transform* transform) 
{
    glm::vec3 cameraPosition = transform->getWorldPosition();
    glm::quat cameraOrientation = transform->getWorldOrientation();

    glm::mat4 rotationMatrix = glm::mat4_cast(glm::conjugate(cameraOrientation));
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0), -cameraPosition);

    m_viewMatrix = rotationMatrix * translationMatrix;
}

void CameraComponent::calculateProjectionMatrix() 
{
    this->m_projectionMatrix = glm::perspective(glm::radians(fieldOfView), )    
}

glm::mat4 CameraComponent::getViewMatrix() { return glm::mat4(); }

glm::mat4 CameraComponent::getProjectionMatrix() { return glm::mat4(); }
