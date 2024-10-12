#pragma once
#include <vector>
#include "Resources/Primitives.h"

class Mesh
{
public:
    inline void AddPrimitive(Primitive pPrimitive) { mPrimitives.push_back(pPrimitive); }
    void Draw();
private:
    std::vector<Primitive> mPrimitives;
};

