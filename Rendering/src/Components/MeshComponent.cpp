#include "MeshComponent.h"

MeshComponent::MeshComponent(Entity *mEntity) : Component(mEntity)
{
    this->componentType = ComponentType::Mesh;
}

MeshComponent::~MeshComponent() {}

void MeshComponent::setMesh(Mesh *mesh) 
{
    this->mesh = mesh;
}

Mesh* MeshComponent::getMesh() {
    return this->mesh;
}

MeshComponent::MeshComponent(const MeshComponent& other)
    : Component(other.mEntity), mesh(other.mesh) 
{
    this->componentType = ComponentType::Mesh;
}

Component* MeshComponent::clone() const {
    return new MeshComponent(*this);
}