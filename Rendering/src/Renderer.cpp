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
    Entity* mainCamera = EntityManager::getInstance().findFirstEntityByDisplayName("Main Camera");
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
    glGenTextures(1, &shadowMapTextureArray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, shadowMapTextureArray);
    // Always set reasonable texture parameters
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}

void Renderer::Update(float deltaTime)
{
    // TODO: Collect set of renderables here and use it.
    // TODO: Lock Enttiies while grabbing
    FetchRenderables();
    FetchLights();
    FetchSkybox();
    {
        std::lock_guard<std::mutex> lock(windowRef->mMutex);
        windowRef->SetWindowToCurrentThread();

        // TODO: Revision, change such that renderables are taken and threads are spun up to do tasks within Renderer.
        GenerateShadowMaps();

        // Set FrameBuffer
        int width, height;
        glfwGetWindowSize(windowRef->GetWindow(), &width, &height);
        mFrameBuffer.Resize(width, height);

        // Set FrameBuffer
        mFrameBuffer.Bind();
        RenderToFrame(width, height);
        mFrameBuffer.Unbind();

        //Perform Post Processing
        //Draw Frame Buffer
        WriteToFrameBuffer();
    }

    windowRef->SwapWindowBuffers();

    for (auto ptr : renderablesThisFrame)
    {
        delete ptr;
    }
    renderablesThisFrame.clear();
    lightsThisFrame.clear();
}

void Renderer::FixedUpdate() {}

void Renderer::RenderToFrame(int pWidth, int pHeight)
{
    // Clear color and depth buffers for set Framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    SetEngineUBO(pWidth, pHeight);
    //TODO: Replace with Scene based or Material based Draw
    IssueMeshDrawCalls();

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

    skyboxThisFrame->Bind();
    skyboxThisFrame->Draw();
    //shaderStorageBufferObject.Unbind();
}

void Renderer::WriteToFrameBuffer()
{
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind Screen Shader
    mScreenShader->Use();
    // Bind Frame Texture
    mFrameBuffer.BindFrameTexture();
    // Draw Screen Quad
    mScreenQuad.Draw();
}

void Renderer::IssueMeshDrawCalls()
{
    auto meshComponentUUIDs = EntityManager::getInstance().findEntitiesByComponent(ComponentType::Mesh);

    for (auto& uuid : meshComponentUUIDs)
    {
        auto entity = EntityManager::getInstance().getEntity(uuid);
        if (entity != nullptr)
        {
            MeshComponent* meshComponent = dynamic_cast<MeshComponent*>(entity->getComponent(ComponentType::Mesh));

            if (meshComponent != nullptr)
            {
                glm::mat4 modelMatrix = entity->transform->getTransformMatrix();
                mEngineUniformBuffer.SetSubData(modelMatrix, 0);
                meshComponent->getMesh()->DrawWithOwnMaterial();
            }
        }
    }
}

void Renderer::SetEngineUBO(int pWidth, int pHeight)
{
    // CAMERA =====================
    CameraComponent* cameraComponent = dynamic_cast<CameraComponent*>(mainCameraEntity->getComponent(ComponentType::Camera));

    if (cameraComponent != nullptr)
    {
        // Update Aspect Ratio if the window has resized
        cameraComponent->updateAspectRatio(pWidth, pHeight);

        cameraComponent->calculateViewMatrix(cameraComponent->entity->transform);
        cameraComponent->calculateProjectionMatrix();
        mEngineUniformBuffer.SetCameraMatrices(
            cameraComponent->getViewMatrix(),
            cameraComponent->getProjectionMatrix(),
            mainCameraEntity->transform->getWorldPosition()
        );
    }

    BindShadowMaps();
}

void Renderer::GenerateShadowMaps()
{
    int numOfLights = lightsThisFrame.size();

    std::vector<glm::mat4> lightMatricies(numOfLights);
    std::vector<glm::mat4> lightViewMatricies(numOfLights);
    std::vector<glm::mat4> lightEnabled(numOfLights); // I also hate this but the SSBO fields not generated properly with bool, int, or vec2 types

    glBindTexture(GL_TEXTURE_2D_ARRAY, shadowMapTextureArray);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_DEPTH_COMPONENT16, MAX_SHADOW_SIZE, MAX_SHADOW_SIZE, numOfLights);
    for (int i = 0; i < numOfLights; ++i)
    {
        auto lightComponent = lightsThisFrame[i];
        lightMatricies[i] = lightComponent->GenerateMatrix(lightComponent->entity->transform);

        glm::vec3 lightPosition = lightComponent->entity->transform->getWorldPosition();
        glm::quat lightOrientation = lightComponent->entity->transform->getWorldOrientation();

        glm::mat4 rotationMatrix = glm::mat4_cast(glm::conjugate(lightOrientation));
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0), -lightPosition);

        glm::mat4 viewMatrix = rotationMatrix * translationMatrix;
        glm::mat4 projectionMatrix;
        if (lightComponent->GetType() == LightType::DirectionalLight)
        {
            projectionMatrix = lightComponent->GetOrthoMatrix();
        }
        else
        {
            projectionMatrix = lightComponent->GetPerspectiveMatrix();
        }

        mEngineUniformBuffer.SetCameraMatrices(
            viewMatrix,
            projectionMatrix,
            lightComponent->entity->transform->getWorldPosition()
        );


        lightEnabled[i] = glm::mat4(lightComponent->GetCreatesShadows());
        if (!lightEnabled[i][0][0]) continue;
        lightViewMatricies[i] = projectionMatrix * viewMatrix;
        lightComponent->BindShadowFrameBuffer();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        mShadowMapShader->Use();
        for (auto renderable : renderablesThisFrame)
        {
            glm::mat4 modelMatrix = renderable->worldTransform->getTransformMatrix();

            mEngineUniformBuffer.SetSubData(modelMatrix, 0);
            renderable->mesh->Draw();
        }

        glCopyImageSubData(
            lightComponent->GetShadowMapID(), GL_TEXTURE_2D, 0, 0, 0, 0,
            shadowMapTextureArray, GL_TEXTURE_2D_ARRAY, 0, 0, 0, i,
            MAX_SHADOW_SIZE, MAX_SHADOW_SIZE, 1);
    }

    mLightMatriciesSSBO.SendBlocks(lightMatricies.data(), lightMatricies.size() * sizeof(glm::mat4));
    mLightViewsSSBO.SendBlocks(lightViewMatricies.data(), lightViewMatricies.size() * sizeof(glm::mat4));
    mLightsEnabledSSBO.SendBlocks(lightEnabled.data(), lightEnabled.size() * sizeof(glm::mat4));

    mLightMatriciesSSBO.Bind(0);
    mLightViewsSSBO.Bind(1);
    mLightsEnabledSSBO.Bind(2);
}

void Renderer::BindShadowMaps()
{
    glActiveTexture(GL_TEXTURE31);
    glBindTexture(GL_TEXTURE_2D_ARRAY, shadowMapTextureArray);
}

void Renderer::FetchLights()
{
    std::vector<glm::mat4> lightMatricies;
    auto lightComponentUUIDs = EntityManager::getInstance().findEntitiesByComponent(ComponentType::Light);
    for (auto& uuid : lightComponentUUIDs)
    {
        auto entity = EntityManager::getInstance().getEntity(uuid);
        LightComponent* lightComponent = dynamic_cast<LightComponent*>(entity->getComponent(ComponentType::Light));
        if (lightComponent == nullptr) continue;
        lightsThisFrame.push_back(lightComponent);
    }

}

void Renderer::FetchSkybox()
{
    auto skyboxID = Core::getInstance().GetScene()->GetSkyboxID();
    skyboxThisFrame = AssetManager::GetInstance().GetSkyboxByID(skyboxID);
}

void Renderer::FetchRenderables()
{
    ComponentMask renderableComponentMask;
    // TODO: Add Materials to this.
    renderableComponentMask.set(static_cast<size_t>(ComponentType::Mesh));
    renderableComponentMask.set(static_cast<size_t>(ComponentType::Transform));
    auto renderableEntities = EntityManager::getInstance().findEntitiesContainingComponentMask(renderableComponentMask);

    for (auto renderEntity : renderableEntities)
    {
        TransformComponent* transformComponent = new TransformComponent(*(renderEntity->transform));
        MeshComponent* meshComponent = dynamic_cast<MeshComponent*>(renderEntity->getComponent(ComponentType::Mesh));
        // TODO: MaterialComponent pointer here.
        Material* materialComponent = nullptr;

        Renderable* renderable = new Renderable(transformComponent);
        if (meshComponent != nullptr && meshComponent->getMesh() != nullptr)
        {
            renderable->mesh = meshComponent->getMesh();
        }

        renderablesThisFrame.push_back(renderable);
    }
}