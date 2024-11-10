#pragma once

#include "System.h"
#include "Entity.h"
#include "Buffers/FrameBuffer.h"
#include "Buffers/UniformBuffer.h"
#include "Buffers/ShaderStorageBuffer.h"
#include "Data/Shape.h"
#include "Resources/Shader.h"
#include "Resources/Primitives.h"
#include "Window/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// @TODO: Remove TESTING INCLUDE
//#include "Resources/Mesh.h"
//#include "Components/TransformComponent.h"
//#include "Components/CameraComponent.h"
//#include "Entities/CameraEntity.h"

static const std::string SCREEN_SHADER_PATH = "../Rendering/Shaders/screen.glsl";

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
    void RenderFrame();

    // GARBAGE BLOCK HATE IT
    // TODO: Decouple from member to scene reference
    //Mesh testMesh;
    //TransformComponent* testTransform = nullptr;
    //std::vector<UUIDv4::UUID> testMaterials;

private:
    Window* windowRef;

    Entity* mainCameraEntity = nullptr; 
    std::unique_ptr<Shader> mScreenShader = std::make_unique<Shader>(SCREEN_SHADER_PATH, "ScreenShader");
    Primitive mScreenQuad;
    FrameBuffer mFrameBuffer;

    UniformBuffer mEngineUniformBuffer;
    ShaderStorageBuffer shaderStorageBufferObject;

    void DrawByMesh();
};
