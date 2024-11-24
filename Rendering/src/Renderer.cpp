#include "Renderer.h"

#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"
#include "Components/MeshComponent.h"
#include "Core.h"
#include "Scene.h"
#include "Managers/AssetManager.h"
#include "Managers/EntityManager.h"
#include "Resources/Mesh.h"

#include <glm.hpp>

Renderer::Renderer(Window* window) : mScreenQuad(Quad.vertices, Quad.indices)
{
    windowRef = window;
}

Renderer::~Renderer() {}

void Renderer::Initialize()
{
    System::Initialize();

    // Get Camera if it exists
    auto mainCameraUUID = EntityManager::getInstance().findFirstEntityByDisplayName("Main Camera");
    auto mainCamera = EntityManager::getInstance().getEntity(mainCameraUUID);
    if (mainCamera != nullptr)
    {
        this->mainCameraEntity = mainCamera;
    }
    else
    {
        std::cerr << "Error: Main Camera not found. The program will now exit." << std::endl;
        std::cin.get();
        std::exit(EXIT_FAILURE);  // Terminate program with failure status
    }
}

void Renderer::Update(float deltaTime)
{
    windowRef->SetWindowToCurrentThread();
    int width, height;
    glfwGetWindowSize(windowRef->GetWindow(), &width, &height);
    mFrameBuffer.Resize(width, height);

    // Set FrameBuffer
    mFrameBuffer.Bind();
    RenderToFrame(width, height);
    mFrameBuffer.Unbind();

    //Perform Post Processing and Draw Frame Buffer
    RenderFrame();

    // Swap window buffers. can be moved to post update
    windowRef->SwapWindowBuffers();
}

void Renderer::FixedUpdate() {}

void Renderer::RenderToFrame(int pWidth, int pHeight)
{
    // Clear color and depth buffers for set Framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    SetEngineUBO(pWidth, pHeight);
    //TODO: Replace with Scene based or Material based Draw
    DrawByMesh();

    // ============================
    //Material Based:
    //For each Material
    //	Bind Material
    //	Apply Material specific Uniforms
    //	For each Primitive in Material
    //		Bind Vertex Array
    //		Bind Index Buffer
    //		DrawCall
    //	Unbind Material
    //	

    //shaderStorageBufferObject.Unbind();
}

void Renderer::RenderFrame()
{
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind Screen Shader
    mScreenShader->Use();
    //AssetManager::GetInstance().GetDefaultShader()->Use();
    // Bind Frame Texture
    mFrameBuffer.BindFrameTexture();
    // Draw Screen Quad
    mScreenQuad.Draw();
}

void Renderer::DrawByMesh()
{
    auto meshComponentUUIDs = EntityManager::getInstance().findEntitiesByComponent(ComponentType::Mesh);

    for (auto& uuid : meshComponentUUIDs)
    {
        auto entity = EntityManager::getInstance().getEntity(uuid);
        MeshComponent* meshComponent = dynamic_cast<MeshComponent*>(entity->getComponent(ComponentType::Mesh));

        if (meshComponent != nullptr)
        {
            // TODO: Bug Physics/Core on way to get modelMatrix directly from transform
            glm::mat4 modelMatrix = entity->transform->getTransformMatrix();
            mEngineUniformBuffer.SetSubData(modelMatrix, 0);
            meshComponent->getMesh()->Draw();
        }
    }
}

void Renderer::SetEngineUBO(int pWidth, int pHeight)
{
    std::vector<glm::mat4> lightMatricies;
    auto lightComponentUUIDs = EntityManager::getInstance().findEntitiesByComponent(ComponentType::Light);
    for (auto& uuid : lightComponentUUIDs)
    {
        auto entity = EntityManager::getInstance().getEntity(uuid);
        LightComponent* lightComponent = dynamic_cast<LightComponent*>(entity->getComponent(ComponentType::Light));
        if (lightComponent == nullptr) continue;
        lightMatricies.push_back(lightComponent->GenerateMatrix(lightComponent->mEntity->transform));
    }

    shaderStorageBufferObject.SendBlocks(lightMatricies.data(), lightMatricies.size() * sizeof(glm::mat4));
    shaderStorageBufferObject.Bind(0);

    // CAMERA =====================
    CameraComponent* cameraComponent = dynamic_cast<CameraComponent*>(mainCameraEntity->getComponent(ComponentType::Camera));

    if (cameraComponent != nullptr)
    {
        // Update Aspect Ratio if the window has resized
        cameraComponent->updateAspectRatio(pWidth, pHeight);

        cameraComponent->calculateViewMatrix(cameraComponent->mEntity->transform);
        cameraComponent->calculateProjectionMatrix();
        mEngineUniformBuffer.SetCameraMatrices(
            cameraComponent->getViewMatrix(),
            cameraComponent->getProjectionMatrix(),
            mainCameraEntity->transform->getWorldPosition()
        );
    }
}