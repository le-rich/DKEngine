#pragma once
#include "System.h"
#include "Buffers/UniformBuffer.h"
// @TODO: Remove TESTING INCLUDE
#include "Resources/Mesh.h"
#include "Components/Transform.h"
#include "Components/CameraComponent.h"
#include "Entities/CameraEntity.h"

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

    void FixedUpdate() override;

    // GARBAGE BLOCK HATE IT
    // TODO: Decouple from member to scene reference
    Mesh testMesh;
    Transform* testTransform;
    std::vector<UUIDv4::UUID> testMaterials;

private:
    Entity* mainCameraEntity; 


    UniformBuffer mEngineUniformBuffer;

};
