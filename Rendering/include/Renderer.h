#pragma once

#include "System.h"
#include "Entity.h"
#include "Components/TransformComponent.h"
#include "Resources/Material.h"
#include "Resources/Mesh.h"
#include "Buffers/FrameBuffer.h"
#include "Buffers/UniformBuffer.h"
#include "Buffers/ShaderStorageBuffer.h"
#include "Data/Shape.h"
#include "Resources/Shader.h"
#include "Resources/Skybox.h"
#include "Resources/Primitives.h"
#include "Window/Window.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Forward Decleration
class LightComponent;

static const std::string SCREEN_SHADER_PATH = "../Rendering/Shaders/screen.glsl";
static const std::string SHADOWMAP_SHADER_PATH = "../Rendering/Shaders/shadowmap.glsl";

struct Renderable {
    TransformComponent* worldTransform;
    Mesh* mesh = nullptr;
    Material* material = nullptr; // TODO: To be changed to MaterialComponent.

    Renderable(TransformComponent* transform) : worldTransform(transform) {}

    ~Renderable(){
        // World Transform is the only dynamically created thing for renderables.
        // Mesh and Material is referencing existing objects.
        delete worldTransform;
    }
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

    void FetchLights();
    void FetchRenderables();
    void FetchSkybox();

    const FrameBuffer* GetFrameBuffer() { return &mFrameBuffer; }

private:
    Window* windowRef;

    Entity* mainCameraEntity = nullptr;
    std::unique_ptr<Shader> mScreenShader = std::make_unique<Shader>(SCREEN_SHADER_PATH, "ScreenShader");
    std::unique_ptr<Shader> mShadowMapShader = std::make_unique<Shader>(SHADOWMAP_SHADER_PATH, "ShadowMapShader");
    Primitive mScreenQuad;
    FrameBuffer mFrameBuffer;

    UniformBuffer mEngineUniformBuffer;
    ShaderStorageBuffer mLightMatriciesSSBO;
    ShaderStorageBuffer mLightViewsSSBO;
    ShaderStorageBuffer mLightsEnabledSSBO;
    GLuint shadowMapTextureArray;

    std::vector<LightComponent*> lightsThisFrame;
    std::vector<Renderable*> renderablesThisFrame;
    std::shared_ptr<Skybox> skyboxThisFrame;

    ImGuiContext* imguiContext;

    void IssueMeshDrawCalls();
    void SetEngineUBO(int pWidth, int pHeight);
    void GenerateShadowMaps();
    void BindShadowMaps();
};