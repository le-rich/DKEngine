#pragma once
#include "Component.h"
#include "Resources/Primitives.h"

#include <vector>

class Mesh;

class MeshComponent : public Component
{

public:
    MeshComponent(Entity* mEntity);
    ~MeshComponent();

    MeshComponent(const MeshComponent& other);
    Component* clone() const override;
    
    void setMesh(Mesh* mesh);
    Mesh* getMesh();
    std::vector<Primitive*> getPrimitives();

    MeshComponent& operator=(MeshComponent& const other);

private:
    Mesh* mesh = nullptr;

};
