#include "Resources/Primitives.h"
#include "Managers/AssetManager.h"

Primitive::Primitive(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const UUIDv4::UUID& pMaterialID) :
    mLoadedVertices(vertices), mLoadedIndices(indices), mMaterialID(pMaterialID)
{
    // vector size * 3 pos coords * float data type
    mVertexBuffer = std::make_shared<VertexBuffer>();

    mIndexBuffer = std::make_shared<IndexBuffer>();

    mVertexBuffer->Init(vertices.data(), vertices.size() * sizeof(Vertex));
    mIndexBuffer->Init(indices.data(), indices.size());

    mVertexBufferLayout.Push<float>(3); // 3: x, y, z	pos coords
    mVertexBufferLayout.Push<float>(3); // 3: x, y, z	normal coords
    mVertexBufferLayout.Push<float>(2); // 3: x, y		uv coords

    mVertexArray = std::make_shared<VertexArray>();
    mVertexArray->AddBuffer(*mVertexBuffer, mVertexBufferLayout);
}

Primitive::~Primitive() {}

// Renders primitive
// Does not change bind material on draw.
// TODO: Consider additional draw function that sets material before draw.
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
