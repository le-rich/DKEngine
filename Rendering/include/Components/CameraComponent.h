#pragma once
#include "Component.h"
#include "Transform.h"

class CameraComponent : Component
{
public:
	private Transform* lookAtTarget;

	float fieldOfView = 90;
	float farClipPlane = 1000f;
	float nearClipPlane = 0.3f;

	CameraComponent();
	~CameraComponent();
private:
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;
};
