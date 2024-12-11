#pragma once
#include "Resources/Primitives.h"
#include <vector>

class Mesh
{
public:
    inline void AddPrimitive(Primitive &&pPrimitive) { mPrimitives.emplace_back(std::move(pPrimitive)); }
    void Draw();
    void DrawWithOwnMaterial();
    std::vector<Primitive> GetPrimitives();
private:
    std::vector<Primitive> mPrimitives;
};

