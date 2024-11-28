#pragma once
#include "Component.h"

#include "Components/TransformComponent.h"

class CameraComponent : public Component
{
public:
    float fieldOfView = 90;
    float farClipPlane = 1000.0f;
    float nearClipPlane = 0.3f;
    float aspectRatio = 1;

    CameraComponent(Entity* mEntity);
    CameraComponent(const CameraComponent& other);
    ~CameraComponent();

    Component* clone() const override;

    void calculateViewMatrix(TransformComponent* transform);
    void calculateProjectionMatrix();
    void updateAspectRatio(int width, int height);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    float getFieldOfView() { return fieldOfView; }
    float getFarClipPlane() { return farClipPlane; }
    float getNearClipPlane() { return nearClipPlane; }
    float getAspectRatio() { return aspectRatio; }

    void setFieldOfView(float fov) { fieldOfView = fov; }
    void setFarClipPlane(float fcp) { farClipPlane = fcp; }
    void setNearClipPlane(float ncp) { nearClipPlane = ncp; }
    void setAspectRatio(float ar) { aspectRatio = ar; }

    CameraComponent& operator=(CameraComponent& const other);
private:
    glm::mat4 m_viewMatrix{};
    glm::mat4 m_projectionMatrix{};

};
