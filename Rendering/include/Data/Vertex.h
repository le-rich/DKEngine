#pragma once

#include <glm.hpp>


struct Vertex
{
	glm::vec3 mPosition;

	Vertex() : mPosition(glm::vec3(0.0f, 0.0f, 0.0f)) {};
	Vertex(const glm::vec3& pos) : mPosition(pos) {};
};

