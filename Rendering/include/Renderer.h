#pragma once

#include "System.h"
#include "Buffers/FrameBuffer.h"
#include "Buffers/UniformBuffer.h"
#include "Buffers/ShaderStorageBuffer.h"
#include "Window/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// @TODO: Remove TESTING INCLUDE
#include "Resources/Mesh.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"
#include "Entities/CameraEntity.h"



class Renderer : public System
{
public:
    Renderer(Window* window);
    ~Renderer();

    void Initialize() override;

    void Update(float deltaTime) override;

    const char* GetName() const override {
        return "Renderer";
    }

    void FixedUpdate() override;

    void RenderToFrame();

    // GARBAGE BLOCK HATE IT
    // TODO: Decouple from member to scene reference
    Mesh testMesh;
    TransformComponent* testTransform = nullptr;
    std::vector<UUIDv4::UUID> testMaterials;

private:
    //GLFWwindow* m_Window;
    Window* windowRef;
    Entity* mainCameraEntity = nullptr; 
    FrameBuffer mRenderFrameBuffer;
    UniformBuffer mEngineUniformBuffer;
    ShaderStorageBuffer shaderStorageBufferObject;

    void DrawByMesh();
};
