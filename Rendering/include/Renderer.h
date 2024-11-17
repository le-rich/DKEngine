#pragma once

#include "System.h"
#include "Entity.h"
#include "Components/TransformComponent.h"
#include "Buffers/FrameBuffer.h"
#include "Buffers/UniformBuffer.h"
#include "Buffers/ShaderStorageBuffer.h"
#include "Data/Shape.h"
#include "Resources/Shader.h"
#include "Resources/Primitives.h"
#include "Window/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static const std::string SCREEN_SHADER_PATH = "../Rendering/Shaders/screen.glsl";

struct Renderable {
    TransformComponent worldTransform;
    Mesh* mesh;
    Material* material; //TODO: To be changed to MaterialComponent.
};

class Renderer : public System
{
public:
    Renderer(Window* window);
    ~Renderer();

    void Initialize() override;

    void Update(float deltaTime) override;

    const char* GetName() const override
    {
        return "Renderer";
    }

    void FixedUpdate() override;

    void RenderToFrame(int pWidth, int pHeight);
    void WriteToFrameBuffer();
    void FetchRenderables();

    const FrameBuffer* GetFrameBuffer() { return &mFrameBuffer; }

private:
    Window* windowRef;

    Entity* mainCameraEntity = nullptr;
    std::unique_ptr<Shader> mScreenShader = std::make_unique<Shader>(SCREEN_SHADER_PATH, "ScreenShader");
    Primitive mScreenQuad;
    FrameBuffer mFrameBuffer;

    UniformBuffer mEngineUniformBuffer;
    ShaderStorageBuffer shaderStorageBufferObject;

    void IssueMeshDrawCalls();
    void SetEngineUBO(int pWidth, int pHeight);
};