#include "Renderer.h"

#include "Components/MeshComponent.h"
#include "Components/LightComponent.h"
#include "Core.h"
#include "Scene.h"
#include "Managers/AssetManager.h"
#include "Managers/EntityManager.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

#include <iostream>
#include <cstdlib>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

Renderer::Renderer(Window* window)
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
	updateThread = new std::thread([&]()
		{
			auto previousTime = std::chrono::high_resolution_clock::now();
			auto window = windowRef->GetWindow();
			while (!glfwWindowShouldClose(window))
			{
				printf("RENDERLOOP"); // Commenting this line causes stuttering of the renderthread.
				//windowRef->PollEvents();
				auto currentTime = std::chrono::high_resolution_clock::now();
				std::chrono::duration<float> deltaTime = currentTime - previousTime;
				auto deltaTimeFloatSeconds = deltaTime.count();
				previousTime = currentTime;
				Update(deltaTimeFloatSeconds);
			}
		});
}

void Renderer::Update(float deltaTime)
{
	// TODO: Create copy of Scene Graph/Entity manager to avoid race conditions with other threads
	{
		std::lock_guard<std::mutex> lock(windowRef->mMutex);
		windowRef->SetWindowToCurrentThread();
		// Set FrameBuffer
		RenderToFrame();

		//Perform Post Processing
		//Draw Frame Buffer
		glfwMakeContextCurrent(NULL);
	}

	// Swap window buffers. can be moved to post update
	windowRef->SwapWindowBuffers();
}

void Renderer::FixedUpdate() {}

void Renderer::RenderToFrame()
{
	// Clear color and depth buffers for set Framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

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
		int width, height;
		glfwGetWindowSize(windowRef->GetWindow(), &width, &height);
		cameraComponent->updateAspectRatio(width, height);

		cameraComponent->calculateViewMatrix(cameraComponent->entity->transform);
		cameraComponent->calculateProjectionMatrix();
		mEngineUniformBuffer.SetCameraMatrices(
			cameraComponent->getViewMatrix(),
			cameraComponent->getProjectionMatrix(),
			mainCameraEntity->transform->getWorldPosition()
		);
	}
	// ============================

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

    shaderStorageBufferObject.Unbind();
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
			glm::vec3 localScale = entity->transform->getLocalScale();

            // TODO: Bug Physics/Core on way to get modelMatrix directly from transform
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), entity->transform->getWorldPosition()) *
                glm::mat4_cast(entity->transform->getLocalOrientation()) *
                glm::scale(glm::mat4(1.0f), localScale);
            mEngineUniformBuffer.SetSubData(modelMatrix, 0);
            meshComponent->getMesh()->Draw();
        }
    }
}





