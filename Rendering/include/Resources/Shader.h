#pragma once
#include <glm.hpp>
#include "Resources/Asset.h"

class Shader : public Asset
{
    enum ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    struct ShaderProgramSource
    {
        std::string VertexSource;
        std::string FragmentSource;
    };

public:
    Shader(std::string pFilePath);
    Shader(std::string pFilePath, std::string pShaderName);
    ~Shader();

    void Use();
    void CleanUp();

    void setMat4(const std::string& name, const glm::mat4& mat) const;
    

    Shader& operator=(const Shader& other);

private:
    unsigned int mShaderID;

    ShaderProgramSource ParseShader(const std::string& filePath);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int CompileShader(unsigned int type, const std::string& source);
};

