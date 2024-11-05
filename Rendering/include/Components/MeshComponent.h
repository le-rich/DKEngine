#include "Component.h"

class Mesh;

class MeshComponent : public Component
{

public:
    MeshComponent(Entity* mEntity);
    ~MeshComponent();
    
    void setMesh(Mesh* mesh);
    Mesh* getMesh();

private:
    Mesh* mesh = nullptr;

};
