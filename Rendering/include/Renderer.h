#pragma once
#include "System.h"
#include "Buffers/UniformBuffer.h"
// @TODO: Remove TESTING INCLUDE
#include "Resources/Mesh.h"
#include "Components/Transform.h"

#include "Window/Window.h"
//#include "GLFW/include/GLFW/glfw3.h"

class Renderer : public System
{
public:
    Renderer();
    ~Renderer();

    void Update() override;

    const char* GetName() const override {
        return "Renderer";
    }


    void FixedUpdate() override {
        // Update Loop logic here

    }

    // GARBAGE BLOCK HATE IT
    // TODO: Decouple from member to scene reference
    Mesh testMesh;
    Transform* testTransform;
    //Texture* texture;
    //Shader* testShader;
    Window* windowRef;
    std::vector<UUIDv4::UUID> testMaterials;

private:
    UniformBuffer mEngineUniformBuffer;

};
