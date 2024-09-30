#include "Primitives.h"

Primitive::Primitive(const std::vector<Vertex>& vertices, 
	const std::vector<GLuint>& indices) :
	mLoadedVertices(vertices), mLoadedIndices(indices), mMaterialID(0)
{
	// vector size * 3 pos coords * float data type
	mVertexBuffer.Init(vertices.data(), vertices.size() * 3 * sizeof(float));
	mIndexBuffer.Init(indices.data(), indices.size());

	mVertexBufferLayout.Push<float>(3); // 3: x, y, z pos coords
	mVertexArray.AddBuffer(mVertexBuffer, mVertexBufferLayout);
}

Primitive::~Primitive()
{
}

void Primitive::Draw()
{
	mVertexArray.Bind();
	mIndexBuffer.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, mIndexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}
