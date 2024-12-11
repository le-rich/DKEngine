#pragma once
#include "Data/Vertex.h"

#include <glm.hpp>
#include <vector>

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
        {{glm::vec3(-1.f, -1.f, -1.f)},   {glm::vec3(1.0f, 1.f, 1.f)}, {glm::vec2(0.0, 0.0)}},  // Front Bot Left
        {{glm::vec3(1.f, -1.f, -1.f)},    {glm::vec3(1.0f, 1.f, 1.f)}, {glm::vec2(1.0, 0.0)}},  // Front Bot Right
        {{glm::vec3(1.f, 1.f, -1.f)},     {glm::vec3(1.0f, 1.f, 1.f)}, {glm::vec2(1.0, 1.0)}},  // Front Top Right
        {{glm::vec3(-1.f, 1.f, -1.f)},    {glm::vec3(1.0f, 1.f, 1.f)}, {glm::vec2(0.0, 1.0)}},  // Front Top Left
        {{glm::vec3(-1.f, -1.f, 1.f)},  {glm::vec3(1.f, 1.0f, 1.f)}, {glm::vec2(1.0, 0.0)}},  // Back Bot Left
        {{glm::vec3(1.f, -1.f, 1.f)},   {glm::vec3(1.f, 1.0f, 1.f)}, {glm::vec2(0.0, 1.0)}},  // Back Bot Right
        {{glm::vec3(1.f, 1.f, 1.f)},    {glm::vec3(1.f, 1.0f, 1.f)}, {glm::vec2(1.0, 1.0)}},  // Back Top Right
        {{glm::vec3(-1.f, 1.f, 1.f)},   {glm::vec3(1.f, 1.0f, 1.f)}, {glm::vec2(0.0, 0.0)}}   // Back Top Left
    },
    {
         0, 1, 3,
         3, 1, 2,
         1, 5, 2,
         2, 5, 6,
         5, 4, 6,
         6, 4, 7,
         4, 0, 7,
         7, 0, 3,
         3, 2, 7,
         7, 2, 6,
         4, 5, 0,
         0, 5, 1
    }
    });