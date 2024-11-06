#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "System.h"
#include "Buffers/UniformBuffer.h"
#include "Window/Window.h"
// @TODO: Remove TESTING INCLUDE
#include "Resources/Mesh.h"
#include "Components/Transform.h"
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

    // GARBAGE BLOCK HATE IT
    // TODO: Decouple from member to scene reference
    Mesh testMesh;
    Transform* testTransform = nullptr;
    std::vector<UUIDv4::UUID> testMaterials;

private:
    //GLFWwindow* m_Window;
    Window* windowRef;


    Entity* mainCameraEntity = nullptr; 


    UniformBuffer mEngineUniformBuffer;

};
