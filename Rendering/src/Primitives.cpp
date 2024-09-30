#include "Primitives.h"

Primitive::Primitive(const std::vector<Vertex>& vertices, 
	const std::vector<GLuint>& indices) :
	mLoadedVertices(vertices), mLoadedIndices(indices), mMaterialID(0)
{
	// vector size * 3 pos coords * float data type
	mVertexBuffer.Init(vertices.data(), vertices.size() * 3 * sizeof(float));
	mIndexBuffer.Init(indices.data(), indices.size());
}

Primitive::~Primitive()
{
}
