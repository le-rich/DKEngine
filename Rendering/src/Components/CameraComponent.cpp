#include "Components/CameraComponent.h"

CameraComponent::CameraComponent(Entity* mEntity) : Component(mEntity)
{

}

CameraComponent::CameraComponent(CameraComponent const& other) : Component(other.entity),
lookAtTarget((Transform*)other.lookAtTarget->clone()), fieldOfView(other.fieldOfView), farClipPlane(other.farClipPlane), nearClipPlane(other.nearClipPlane)
{}

CameraComponent::~CameraComponent()
{
}

Component* CameraComponent::clone()
{
	return (new CameraComponent(*this));
}
