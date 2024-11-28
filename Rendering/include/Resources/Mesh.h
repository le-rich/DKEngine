#pragma once
#include <vector>
#include "Resources/Primitives.h"

class Mesh
{
public:
    inline void AddPrimitive(Primitive &&pPrimitive) { mPrimitives.emplace_back(std::move(pPrimitive)); }
    void Draw();
    void DrawWithOwnMaterial();
private:
    std::vector<Primitive> mPrimitives;
};

