#pragma once
#include "System.h"
#include "Buffers/UniformBuffer.h"
#include "Window/Window.h"
// @TODO: Remove TESTING INCLUDE
#include "Resources/Mesh.h"
#include "Components/Transform.h"


class Renderer : public System
{
public:
    Renderer();
    ~Renderer();

    void Initialize() override;

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
    std::vector<UUIDv4::UUID> testMaterials;
    Window* windowRef;

private:
    UniformBuffer mEngineUniformBuffer;

};
