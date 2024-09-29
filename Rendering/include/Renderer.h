#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <string>

#include "Primitives.h"
#include "Renderer.h"
#include "System.h"

// Macro definitions for error handling
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

class Renderer : public System
{
public:
    static const std::string VERTEX_SHADER;
    static const std::string FRAGMENT_SHADER;

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

    void AddEntity(int entityId) override {
        std::cout << "Added entity " << entityId << " to Renderer System." << std::endl;
    }

    const char* GetName() const override {
        return "Renderer";
    }

    void RemoveEntity(int entityId) override {
        std::cout << "Removed entity " << entityId << " from Renderer System." << std::endl;
    }


private:
    static unsigned int CompileShader(unsigned int type, const std::string& source);
    static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    static ShaderProgramSource ParseShader(const std::string& filePath);
};

// Error handling functions
static void GLClearError();
static bool GLLogCall(const char* function, const char* file, int line);

#endif
