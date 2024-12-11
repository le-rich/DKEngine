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

    inline glm::vec3 GetRightVector() { return glm::normalize(glm::vec3(m_viewMatrix[0][0], m_viewMatrix[1][0], m_viewMatrix[2][0])); }
    inline glm::vec3 GetUpVector() { return glm::normalize(glm::vec3(m_viewMatrix[0][1], m_viewMatrix[1][1], m_viewMatrix[2][1])); }
    inline glm::vec3 GetForwardVector() { return glm::normalize(glm::vec3(m_viewMatrix[0][2], m_viewMatrix[1][2], m_viewMatrix[2][2])); }

    bool CheckIfPointExistsInFrustum(glm::vec3 worldPoint);

    CameraComponent& operator=(CameraComponent& const other);
private:
    glm::mat4 m_viewMatrix{};
    glm::mat4 m_projectionMatrix{};
};
