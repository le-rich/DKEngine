#include "MeshComponent.h"

MeshComponent::MeshComponent(Entity *mEntity) : Component(mEntity)
{
    this->componentType = ComponentType::Mesh;
}

MeshComponent::~MeshComponent() {}

void MeshComponent::setMesh(Mesh *mesh) {}

Mesh* MeshComponent::getMesh() {
    return this->mesh;
}
