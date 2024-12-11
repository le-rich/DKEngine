#include "Components/CameraComponent.h"

#include "Component.h"
#include "Entity.h"

#include <glad/glad.h>
#include <glm.hpp>
//#include <gtc/quaternion.hpp>

CameraComponent::CameraComponent(Entity* mEntity) : Component(mEntity)
{
    this->componentType = ComponentType::Camera;
}

CameraComponent::~CameraComponent()
{}

void CameraComponent::calculateViewMatrix(TransformComponent* transform)
{
    m_viewMatrix = glm::inverse(transform->getTransformMatrix());
}

void CameraComponent::calculateProjectionMatrix()
{
    this->m_projectionMatrix = glm::perspective(glm::radians(fieldOfView), aspectRatio, nearClipPlane, farClipPlane);
}

void CameraComponent::updateAspectRatio(int width, int height)
{
    this->aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

bool CameraComponent::CheckIfPointExistsInFrustum(glm::vec3 worldPoint)
{
    auto farDist = getFarClipPlane();
    auto nearDist = getNearClipPlane();

    auto camPos = entity->transform->getWorldPosition();
    auto posRel2Camera = worldPoint - camPos;

    auto camForward = -GetForwardVector();
    auto zProjection = glm::dot(posRel2Camera, camForward);
    if (zProjection > farDist || zProjection < nearDist)
    {
        return false;
    }

    auto tang = tan(glm::radians(getFieldOfView()) * 0.5);
    auto camUp = GetUpVector();
    auto yProjection = glm::dot(posRel2Camera, camUp);
    auto minAngle = zProjection * tang;

    if (yProjection > minAngle || yProjection < -minAngle)
    {
        return false;
    }

    auto camRight = GetRightVector();
    auto xProjection = glm::dot(posRel2Camera, camRight);
    minAngle *= getAspectRatio();

    if (xProjection > minAngle || xProjection < -minAngle) 
    {
        return false;
    }
    return true;
}

CameraComponent& CameraComponent::operator=(CameraComponent& const other)
{
    if (this == &other)
    {
        return *this;
    }
    this->entity->GetEntityID() == other.entity->GetEntityID();
}

glm::mat4 CameraComponent::getViewMatrix() { return m_viewMatrix; }

glm::mat4 CameraComponent::getProjectionMatrix() { return m_projectionMatrix; }

CameraComponent::CameraComponent(const CameraComponent& other)
    : Component(other.entity),
    fieldOfView(other.fieldOfView),
    farClipPlane(other.farClipPlane),
    nearClipPlane(other.nearClipPlane),
    aspectRatio(other.aspectRatio),
    m_viewMatrix(other.m_viewMatrix),
    m_projectionMatrix(other.m_projectionMatrix)
{
    this->componentType = ComponentType::Camera;
}

Component* CameraComponent::clone() const
{
    return new CameraComponent(*this);
}