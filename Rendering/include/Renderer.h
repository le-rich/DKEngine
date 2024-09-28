#pragma once

#include <glad/glad.h>
#include <string>

#include "Primitives.h"
#include "Renderer.h"


// Macro definitions for error handling
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

// Error handling functions

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);


class Renderer
{
public:
    GLuint shaderProgram;

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

    void Init();
    void Draw(const Shape& shape);

private:
    static unsigned int CompileShader(unsigned int type, const std::string& source);
    static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    static ShaderProgramSource ParseShader(const std::string& filePath);
};
