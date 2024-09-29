#pragma once

#include <glm.hpp>

struct Vertex
{
	glm::vec3 mPosition;
	glm::vec3 mNormal;
	glm::vec2 mTextureCoordinate;
	glm::vec3 mTangent;
	glm::vec3 mBitTangent;

	Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& texCoord, 
		const glm::vec3& tang, const glm::vec3& bitTang) : mPosition(pos), mNormal(norm), 
		mTextureCoordinate(texCoord), mTangent(tang), mBitTangent(bitTang) {}
};

