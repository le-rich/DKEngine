#pragma once
#include "System.h"
#include "Buffers/UniformBuffer.h"
// @TODO: Remove TESTING INCLUDE
#include "Resources/Mesh.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"
#include "Entities/CameraEntity.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer : public System
{
public:
    Renderer(GLFWwindow* window);
    ~Renderer();

    void Initialize() override;

    void Update(float deltaTime) override;

    const char* GetName() const override {
        return "Renderer";
    }

    void FixedUpdate() override;

    // GARBAGE BLOCK HATE IT
    // TODO: Decouple from member to scene reference
    Mesh testMesh;
    TransformComponent* testTransform = nullptr;
    std::vector<UUIDv4::UUID> testMaterials;

private:
    GLFWwindow* m_Window;


    Entity* mainCameraEntity = nullptr; 


    UniformBuffer mEngineUniformBuffer;

};
