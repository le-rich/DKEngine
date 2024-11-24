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
}

void Renderer::Update(float deltaTime)
{
	// TODO: Collect set of renderables here and use it.
	FetchRenderables();
	FetchSkybox();
	{
		std::lock_guard<std::mutex> lock(windowRef->mMutex);
        windowRef->SetWindowToCurrentThread();

        // TODO: Create copy of Scene Graph/Entity manager to avoid race conditions with other threads
        // TODO: Revision, change such that renderables are taken and threads are spun up to do tasks within Renderer.
        
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

    // Swap window buffers. can be moved to post update
    windowRef->SwapWindowBuffers();

	for (auto ptr : renderablesThisFrame){
		delete ptr;
	}

	renderablesThisFrame.clear();
}

void Renderer::FixedUpdate() {}

void Renderer::RenderToFrame(int pWidth, int pHeight)
{
	// Clear color and depth buffers for set Framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
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
	//AssetManager::GetInstance().GetDefaultShader()->Use();
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
		lightMatricies.push_back(lightComponent->GenerateMatrix(lightComponent->entity->transform));
	}

	shaderStorageBufferObject.SendBlocks(lightMatricies.data(), lightMatricies.size() * sizeof(glm::mat4));
	shaderStorageBufferObject.Bind(0);

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
	renderableComponentMask |= renderableComponentMask.set(static_cast<size_t>(ComponentType::Mesh) | static_cast<size_t>(ComponentType::Transform));
	auto renderableEntities = EntityManager::getInstance().findEntitiesByComponentMask(renderableComponentMask);

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