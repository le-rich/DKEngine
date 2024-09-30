#include "Primitives.h"


Primitive::Primitive(const std::vector<Vertex>& vertices, 
	const std::vector<GLuint>& indices) :
	mLoadedVertices(vertices), mLoadedIndices(indices)
{
	// vector size * 3 pos coords * float values
	mVertexBuffer.Init(vertices.data(), vertices.size() * 3 * sizeof(float));
	mIndexBuffer.Init(indices.data(), indices.size());
}

Primitive::~Primitive()
{
}
