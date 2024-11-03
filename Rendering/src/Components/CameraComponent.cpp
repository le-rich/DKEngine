#include "Components/CameraComponent.h"

#include <glad/glad.h>

CameraComponent::CameraComponent(Entity* mEntity) : Component(mEntity)
{
    this->componentType = ComponentType::Camera;
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
    this->m_projectionMatrix = glm::perspective(glm::radians(fieldOfView), aspectRatio, nearClipPlane, farClipPlane);    
}

void CameraComponent::updateAspectRatio(int width, int height) 
{
   this-> aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

glm::mat4 CameraComponent::getViewMatrix() { return m_viewMatrix; }

glm::mat4 CameraComponent::getProjectionMatrix() { return m_projectionMatrix; }
