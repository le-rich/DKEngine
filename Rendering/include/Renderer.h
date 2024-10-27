#pragma once
/* @TODO: Extrapolate shader functionality, error handling, and
implement pre and post methods.*/

#include <glad/glad.h>

//#include "Renderer.h"
#include "System.h"
// @TODO: Remove TESTING INCLUDE
#include "Buffers/UniformBuffer.h"
#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "Resources/Shader.h"
#include "Components/Transform.h"




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
    //std::vector<UUIDv4::UUID> testTextures;
    std::vector<UUIDv4::UUID> testMaterials;
    Shader* testShader;
private:
    UniformBuffer mUniformBuffer;

};
