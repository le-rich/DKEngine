#include <glad/glad.h>
#include <glm.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "Renderer.h"
#include "Resources/Texture.h"

// Constructor.
Renderer::Renderer() {}

// Destructor.
Renderer::~Renderer() {}

void Renderer::Update()
{
    // Clear color and depth buffers (can be moved to pre update
     
    /*Get lights
      For each light
        Get lighting matrix
        Add lighting matrix to list
      bind lighting list to Shader Buffer*/

    //For each Material
    //if texture --> bind texture
    //if shader  --> bind shader
    ShaderProgramSource source = ParseShader("../Rendering/Shaders/default.glsl");
    GLuint shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));

    //Texture texture("../Core/Assets/AE86/textures/fmesh_Material_001_BasssseColor_001df.png");
    //texture.Bind();
    //Apply Uniforms (lighting, view matrices, etc...)

    
    /*For each Primitive
        Bind Vertex Array
        Bind Index Buffer
        DrawCall*/

    // GARBAGE METHODOLOGY
    testTextures[0]->Bind();
    testMesh.Draw();
    testTextures[0]->Unbind();

    /*Perform Post Processing
      Draw Frame Buffer*/

    // Swap window buffers. can be moved to post update
    GLCall(glDeleteProgram(shader));
}

// Returns a ID of the compiled shader program on the GPU.
unsigned int Renderer::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // ptr to beginning of data of string

    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) { // did not compile successfully
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

// Returns a shader program.
unsigned int Renderer::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    // Create shaders
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    // Attach shaders to program (link files to program)
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));
    // Delete the shaders since they have been linked to the program
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

// Returns vertex and fragment shader programs from a given path to a file.
Renderer::ShaderProgramSource Renderer::ParseShader(const std::string& filepath) {
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

// Iterates through OpenGL error flags until there are no more.
void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

// Prints to console any openGL error flags with a corresponding function, file, and line number.
bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}