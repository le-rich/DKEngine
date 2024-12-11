#include "Resources/Primitives.h"
#include "Managers/AssetManager.h"

Primitive::Primitive(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices) :
    mLoadedVertices(vertices), mLoadedIndices(indices)
{
    InitPrimitive();
}

Primitive::Primitive(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const UUIDv4::UUID& pMaterialID) :
    mLoadedVertices(vertices), mLoadedIndices(indices), mMaterialID(pMaterialID)
{
    InitPrimitive();
}

Primitive::~Primitive() {}

std::vector<Vertex> Primitive::GetLoadedVertices()
{
    return mLoadedVertices;
}

// Renders primitive
// Does not change bound material on draw.
void Primitive::Draw()
{
    mVertexArray->Bind();
    mIndexBuffer->Bind();
    glDrawElements(GL_TRIANGLES, mIndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
    mIndexBuffer->Unbind();
    mVertexArray->Unbind();
}

void Primitive::DrawWithOwnMaterial()
{
    AssetManager::GetInstance().GetMaterialByID(mMaterialID)->Bind();
    mVertexArray->Bind();
    mIndexBuffer->Bind();
    glDrawElements(GL_TRIANGLES, mIndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
    mIndexBuffer->Unbind();
    mVertexArray->Unbind();
    AssetManager::GetInstance().GetMaterialByID(mMaterialID)->Unbind();
}

void Primitive::InitPrimitive()
{
    // vector size * 3 pos coords * float data type
    mVertexBuffer = std::make_shared<VertexBuffer>();

    mIndexBuffer = std::make_shared<IndexBuffer>();

    mVertexBuffer->Init(mLoadedVertices.data(), mLoadedVertices.size() * sizeof(Vertex));
    mIndexBuffer->Init(mLoadedIndices.data(), mLoadedIndices.size());

    mVertexBufferLayout.Push<float>(3); // 3: x, y, z	pos coords
    mVertexBufferLayout.Push<float>(3); // 3: x, y, z	normal coords
    mVertexBufferLayout.Push<float>(2); // 3: x, y		uv coords

    mVertexArray = std::make_shared<VertexArray>();
    mVertexArray->AddBuffer(*mVertexBuffer, mVertexBufferLayout);
}