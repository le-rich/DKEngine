#pragma once
#include <glm.hpp>
#include <vector>

#include "Data/Vertex.h"

// Base struct for shapes
struct Shape
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

static const Shape Quad({
    {
		{{glm::vec3(-1.f, 1.f, 0.f)},   {glm::vec3(1.0f, 0.5f, 0.5f)}, {glm::vec2(0.f, 1.f)}},  // Top Left
		{{glm::vec3(-1.f, -1.f, 0.f)},  {glm::vec3(1.0f, 0.5f, 0.5f)}, {glm::vec2(0.f, 0.f)}},  // Bot Left
		{{glm::vec3(1.f, -1.f, 0.f)},   {glm::vec3(1.0f, 0.5f, 0.5f)}, {glm::vec2(1.f, 0.f)}},  // Bot Right
		{{glm::vec3(1.f, 1.f, 0.f)},    {glm::vec3(1.0f, 0.5f, 0.5f)}, {glm::vec2(1.f, 1.f)}}   // Top Right
    },
    {
        0, 1, 2,
        0, 2, 3,
    }
    });

// Derived struct for Cube
static const Shape Cube({
    {
        {{glm::vec3(-0.5f, -0.5f, 0.5f)},   {glm::vec3(1.0f, 0.5f, 0.5f)}, {glm::vec2(0.0, 0.0)}},  // Front Bot Left
        {{glm::vec3(0.5f, -0.5f, 0.5f)},    {glm::vec3(1.0f, 0.5f, 0.5f)}, {glm::vec2(1.0, 1.0)}},  // Front Bot Right
        {{glm::vec3(0.5f, 0.5f, 0.5f)},     {glm::vec3(1.0f, 0.5f, 0.5f)}, {glm::vec2(0.0, 1.0)}},  // Front Top Right
        {{glm::vec3(-0.5f, 0.5f, 0.5f)},    {glm::vec3(1.0f, 0.5f, 0.5f)}, {glm::vec2(1.0, 0.0)}},  // Front Top Left
        {{glm::vec3(-0.5f, -0.5f, -0.5f)},  {glm::vec3(0.5f, 1.0f, 0.5f)}, {glm::vec2(1.0, 0.0)}},  // Back Bot Left
        {{glm::vec3(0.5f, -0.5f, -0.5f)},   {glm::vec3(0.5f, 1.0f, 0.5f)}, {glm::vec2(0.0, 1.0)}},  // Back Bot Right
        {{glm::vec3(0.5f, 0.5f, -0.5f)},    {glm::vec3(0.5f, 1.0f, 0.5f)}, {glm::vec2(1.0, 1.0)}},  // Back Top Right
        {{glm::vec3(-0.5f, 0.5f, -0.5f)},   {glm::vec3(0.5f, 1.0f, 0.5f)}, {glm::vec2(0.0, 0.0)}}   // Back Top Left
    },
    {
        // Front
        0, 1, 2,
        0, 2, 3,
        // Back 
        4, 6, 5,
        4, 7, 6,
        // Left 
        0, 4, 7,
        0, 7, 3,
        // Right
        1, 5, 6,
        1, 6, 2,
        // Top
        3, 2, 6,
        3, 6, 7,
        // Bottom
        0, 1, 5,
        0, 5, 4
    }
    });