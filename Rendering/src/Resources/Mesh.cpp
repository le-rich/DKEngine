#include "Resources/Mesh.h"

// Draws mesh
void Mesh::Draw()
{
    for (auto primitive : mPrimitives)
    {
        primitive.Draw();
    }
}

std::vector<Primitive> Mesh::GetPrimitives()
{
    return mPrimitives;
}

void Mesh::DrawWithOwnMaterial()
{
    for (auto primitive : mPrimitives)
    {
        primitive.GetLoadedVertices();
        primitive.DrawWithOwnMaterial();
    }
}
