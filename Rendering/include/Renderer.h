#pragma once
/* @TODO: Extrapolate shader functionality, error handling, and
implement pre and post methods.*/

#include <glad/glad.h>
#include <string>

//#include "Renderer.h"
#include "System.h"
// @TODO: Remove TESTING INCLUDE
#include "Resources/Mesh.h"
#include "Resources/Texture.h"

// Macro definitions for error handlingb
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

// Error handling functions

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);


class Renderer : public System
{
public:
    Renderer();
    ~Renderer();

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    struct ShaderProgramSource
    {
        std::string VertexSource;
        std::string FragmentSource;
    };

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
    // TODO: Decouple from member to AssetManager reference
    std::vector<std::shared_ptr<Texture>> testTextures;
private:
    static unsigned int CompileShader(unsigned int type, const std::string& source);
    static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    static ShaderProgramSource ParseShader(const std::string& filePath);
};
