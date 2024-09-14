#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <Primitives.h>
#include <Renderer.h>

// Static member initialization
const std::string Renderer::vertexShader =
"#version 330 core\n"
"layout(location = 0) in vec4 position;\n"
"void main()\n"
"{\n"
"    gl_Position = position;\n"
"}\n";

const std::string Renderer::fragmentShader =
"#version 330 core\n"
"layout(location = 0) out vec4 color;\n"
"void main()\n"
"{\n"
"    color = vec4(1.0, 0.0, 0.0, 1.0);\n"
"}\n";

// Constructor
Renderer::Renderer() {
    Init();
}

// Destructor
Renderer::~Renderer() {
    GLCall(glDeleteProgram(shaderProgram));
}

void Renderer::Init() {
    shaderProgram = CreateShader(vertexShader, fragmentShader);
}

void Renderer::Draw(const Shape& shape) {
    // Create and bind a Vertex Array Object (VAO)
    GLuint vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    // Send vertex buffer to GPU
    GLuint vbo;
    GLCall(glGenBuffers(1, &vbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, shape.vertices.size() * sizeof(float), shape.vertices.data(), GL_STATIC_DRAW));

    // Send index buffer to GPU
    GLuint ibo; // stands for index buffer object
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indices.size() * sizeof(float), shape.indices.data(), GL_STATIC_DRAW));

    // Use the shader program
    GLCall(glUseProgram(shaderProgram));

    // Layout of buffer
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0));
    GLCall(glEnableVertexAttribArray(0));

    // Unbind the VAO to avoid unintended modifications
    glBindVertexArray(0);

    // Drawing
    GLCall(glBindVertexArray(vao));
    GLCall(glDrawElements(shape.drawMode, shape.indices.size(), GL_UNSIGNED_INT, nullptr));
    GLCall(glBindVertexArray(vao));

    // Cleanup
    GLCall(glDeleteBuffers(1, &vbo));
    GLCall(glDeleteVertexArrays(1, &vao));
    GLCall(glDeleteProgram(shaderProgram));
}

unsigned int Renderer::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile" <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

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

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}