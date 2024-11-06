#include "Component.h"

class Mesh;

class MeshComponent : public Component
{

public:
    MeshComponent(Entity* mEntity);
    ~MeshComponent();

    Component* clone() override;
    
    void setMesh(Mesh* mesh);
    Mesh* getMesh();

    MeshComponent& operator=(MeshComponent& const other);

private:
    Mesh* mesh = nullptr;

};
