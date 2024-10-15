#pragma once
#include "Component.h"
#include "Transform.h"

class CameraComponent : Component
{
public:
	private Transform transform;
	private Transform lookAtTarget;

	float fieldOfView = 90;
	float farClipPlane = 1000f;
	float nearClipPlane = 0.3f;

	void LookAt

private:
	
};
