#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <glad/glad.h> 

// Base struct for shapes
struct Shape
{
    std::vector<float> vertices;
    std::vector<GLuint> indices;
    GLenum drawMode;

    Shape(const std::vector<float>& vertices, const std::vector<GLuint>& indices, GLenum mode)
        : vertices(vertices), indices(indices), drawMode(mode) {}
};

// Derived struct for Triangle
struct Triangle : public Shape
{
    Triangle() : Shape({
        -0.5f, -0.5f, // 0
         0.0f,  0.5f, // 1
         0.5f, -0.5f  // 2
        },
    {
        0, 1, 2
    }, GL_TRIANGLES) {}
};

// Derived struct for Square
struct Square : public Shape
{
    Square() : Shape({
        -0.5f, -0.5f,  // 0
         0.5f, -0.5f,  // 1
         0.5f,  0.5f,  // 2
        -0.5f,  0.5f   // 3
        },
    {
        0, 1, 2,
        2, 3, 0
    }, GL_TRIANGLES) {}
};

#endif
