#include "Buffers/VertexArray.h"
#include "Data/VertexBufferLayout.h"
#include "Utils/Logger.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &mRendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &mRendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();

	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type,
			element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	Unbind();
	vb.Unbind();
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(mRendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
