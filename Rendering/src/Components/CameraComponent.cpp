#include "Components/CameraComponent.h"

CameraComponent::CameraComponent(Entity* mEntity) : Component(mEntity)
{

}

CameraComponent::CameraComponent(CameraComponent const& other) : Component(other.entity)
{
	lookAtTarget = other.lookAtTarget == nullptr ? other.lookAtTarget : ((Transform*)other.lookAtTarget->clone());
	this->fieldOfView = other.fieldOfView;
	this->farClipPlane = other.farClipPlane;
	this->nearClipPlane = other.nearClipPlane;
	this->m_projectionMatrix = other.m_projectionMatrix;
	this->m_viewMatrix = other.m_viewMatrix;
}

CameraComponent::~CameraComponent()
{
}

Component* CameraComponent::clone()
{
	return (new CameraComponent(*this));
}
