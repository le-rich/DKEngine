#pragma once
#include "Resources/Primitives.h"
#include <vector>

class Mesh
{
public:
    inline void AddPrimitive(Primitive *pPrimitive) { mPrimitives.emplace_back(pPrimitive); }
    void Draw();
    void DrawWithOwnMaterial();
    std::vector<Primitive*>& getPrimitives() { return this->mPrimitives; }

private:
    std::vector<Primitive*> mPrimitives;
};

