#pragma once
#include "Component.h"

#include "Components/Transform.h"

class CameraComponent : public Component
{
public:
    Transform* lookAtTarget;

    float fieldOfView = 90;
    float farClipPlane = 1000.0f;
    float nearClipPlane = 0.3f;

    CameraComponent(Entity* mEntity);
    ~CameraComponent();

    void calculateViewMatrix(Transform* transform);
    void calculateProjectionMatrix();

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
private:
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
};
