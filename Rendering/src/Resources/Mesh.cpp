#include "Resources/Mesh.h"

// Draws mesh
void Mesh::Draw()
{
    for (auto primitive : mPrimitives)
    {
        primitive.Draw();
    }
}
