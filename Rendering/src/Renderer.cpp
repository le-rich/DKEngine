#include "Renderer.h"

#include "Components/MeshComponent.h"
#include "Managers/AssetManager.h"
#include "Managers/EntityManager.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

#include <iostream>
#include <cstdlib>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

Renderer::Renderer(GLFWwindow* window)
{
    m_Window = window;
}

Renderer::~Renderer() {}

void Renderer::Initialize() {
    System::Initialize(); 

    // Get Camera if it exists
    auto mainCameraUUID = EntityManager::getInstance().findFirstEntityByDisplayName("Main Camera");
    auto mainCamera = EntityManager::getInstance().getEntity(mainCameraUUID);
    if (mainCamera != nullptr) {
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
    // Clear color and depth buffers (can be moved to pre update

    /*Get lights
      For each light
        Get lighting matrix
        Add lighting matrix to list
      bind lighting list to Shader Buffer*/

    // TODO: Bug Physics/Core on way to get modelMatrix directly from transform
    glm::vec3 localScale = testTransform->getLocalScale();
    glm::mat4 modelMatrix =
        glm::translate(glm::mat4(1.0f), testTransform->getWorldPosition()) *
        glm::mat4_cast(testTransform->getLocalOrientation()) *
        glm::mat4( // creates a scale matrix 
            glm::vec4(localScale.x, 0.0f, 0.0f, 0.0f),
            glm::vec4(0.0f, localScale.y, 0.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, localScale.z, 0.0f),
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
        );

    mEngineUniformBuffer.SetSubData(modelMatrix, 0);


    // CAMERA =====================
    CameraComponent* cameraComponent = dynamic_cast<CameraComponent*>(mainCameraEntity->getComponent(ComponentType::Camera));

    if (cameraComponent != nullptr){
        // Update Aspect Ratio if the window has resized
        int width, height;
        glfwGetWindowSize(m_Window, &width, &height);
        cameraComponent->updateAspectRatio(width, height);

        cameraComponent->calculateViewMatrix(cameraComponent->entity->transform);
        cameraComponent->calculateProjectionMatrix();
    }
    // ============================

    /*
    Material Based:
    For each Material
        Bind Material
        Apply Material specific Uniforms
        For each Primitive in Material
            Bind Vertex Array
            Bind Index Buffer
            DrawCall
        Unbind Material
     */

     // Kinda okay methodology
     //TODO: Replace with Scene based or Material based Draw
    

    // MESHES ============================
    auto meshComponentUUIDs = EntityManager::getInstance().findEntitiesByComponent(ComponentType::Mesh);
    
    for (auto& uuid : meshComponentUUIDs) 
    {
        auto entity = EntityManager::getInstance().getEntity(uuid);
        MeshComponent* meshComponent = dynamic_cast<MeshComponent*>(entity->getComponent(ComponentType::Mesh));

        if (meshComponent != nullptr)
        {
            // glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), entity->transform->getWorldPosition()) *
            //                        glm::mat4_cast(entity->transform->getWorldOrientation()) *
            //                        glm::scale(glm::mat4(1.0f), entity->transform->getWorldScale());
            
            meshComponent->getMesh()->Draw();
        }
    }

    // ===================================

    /*Perform Post Processing
      Draw Frame Buffer*/

      // Swap window buffers. can be moved to post update
}

void Renderer::FixedUpdate() {
    
}





