#include "MeshComponent.h"

#include "Component.h"
#include "Entity.h"

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

MeshComponent& MeshComponent::operator=(MeshComponent& const other)
{
	if (this == &other)
	{
		return *this;
	}

	this->mesh = other.mesh;
	this->entity->GetEntityID() == other.entity->GetEntityID();
}
