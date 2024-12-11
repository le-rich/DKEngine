#pragma once

#include <glm.hpp>
#include <epsilon.hpp>

struct Vertex
{
	glm::vec3 mPosition;
	glm::vec3 mNormal;
	glm::vec2 mUVCoord;

	// Overloaded operator to handle == with Vertices
	bool operator==(const Vertex& other) const {
		const float epsilon = 1e-5f; // Tolerance for floating-point comparison
		return glm::all(glm::epsilonEqual(mPosition, other.mPosition, epsilon)) &&
			   glm::all(glm::epsilonEqual(mNormal, other.mNormal, epsilon)) &&
			   glm::all(glm::epsilonEqual(mUVCoord, other.mUVCoord, epsilon));
	}
};

