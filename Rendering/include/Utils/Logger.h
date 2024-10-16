#pragma once
#include <iostream>
#include <glad/glad.h>

// Macro definitions for error handlingb
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

// Error handling functions
//
//void GLClearError();
//bool GLLogCall(const char* function, const char* file, int line);

// Iterates through OpenGL error flags until there are no more.
static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

// Prints to console any openGL error flags with a corresponding function, file, and line number.
static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}