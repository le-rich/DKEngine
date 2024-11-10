#include "Resources/Shader.h"

#include <fstream>
#include <sstream>
#include <glad/glad.h>

#include "Utils/Logger.h"

Shader::Shader(std::string pFilePath) : Asset()
{
    ShaderProgramSource source = ParseShader(pFilePath);
    mShaderID = CreateShader(source.VertexSource, source.FragmentSource);
}


Shader::Shader(std::string pFilePath, std::string pShaderName)// : Shader(pShaderName) // Delegated constructor causes error when trying to call glUseProgram
{
    ShaderProgramSource source = ParseShader(pFilePath);
    mShaderID = CreateShader(source.VertexSource, source.FragmentSource);
    this->assetDisplayName = pShaderName;
}

Shader::~Shader() {}

void Shader::Use()
{
    GLCall(glUseProgram(mShaderID));
}

void Shader::CleanUp()
{
    GLCall(glDeleteShader(mShaderID));
}

// Returns a ID of the compiled shader program on the GPU.
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // ptr to beginning of data of string

    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    return id;
}

// Returns a shader program.
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    // Create shaders
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    // Attach shaders to program (link files to program)
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // The program is useless now. So delete it.
        glDeleteProgram(program);

        // Provide the infolog in whatever manner you deem best.
        // Exit with failure.
        return program;
    }
    GLCall(glValidateProgram(program));
    // Delete the shaders since they have been linked to the program
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

// Returns vertex and fragment shader programs from a given path to a file.
Shader::ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2]; // stores both vertex and fragment shader
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        // finds type of shader from custom # tags
        if (line.find("#shader") != std::string::npos) // if not invalid string pos since .find returns position of string (size_t)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n'; // dump the contents of the shader program
        }
    }

    return { ss[0].str(), ss[1].str() };
}

Shader& Shader::operator=(const Shader& other)
{
    if (this == &other)
    {
        return *this;
    }

    this->mShaderID = other.mShaderID;

    return *this;
}