#pragma once
#include "Component.h"

#include "Components/TransformComponent.h"

class CameraComponent : public Component
{
public:
	TransformComponent* lookAtTarget = nullptr;

    float fieldOfView = 90;
    float farClipPlane = 1000.0f;
    float nearClipPlane = 0.3f;
    float aspectRatio;

    CameraComponent(Entity* mEntity);
    //CameraComponent(CameraComponent const&);
    ~CameraComponent();

    void calculateViewMatrix(TransformComponent* transform);
    void calculateProjectionMatrix();
    void updateAspectRatio(int width, int height);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    Component* clone() override;

    CameraComponent& operator=(CameraComponent& const other);
private:
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

};
