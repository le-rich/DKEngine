#pragma once
/* @TODO: Extrapolate shader functionality, error handling, and
implement pre and post methods.*/

#include <glad/glad.h>

//#include "Renderer.h"
#include "System.h"
// @TODO: Remove TESTING INCLUDE
#include "Resources/Mesh.h"

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

    Mesh testMesh;
private:

};
