#include "Components/ScriptComponent.h"

#include "Component.h"
#include "Entity.h"

ScriptComponent::ScriptComponent(Entity* mEntity) : Component(mEntity)
{
	this->componentType = ComponentType::Script;
}

ScriptComponent::~ScriptComponent()
{

}

Component* ScriptComponent::clone() 
{
	return (new ScriptComponent(*this));
}

ScriptComponent& ScriptComponent::operator=(ScriptComponent& const other) 
{
	if (this == &other)
	{
		return *this;
	}
	this->scripts = other.scripts;
	this->entity->GetEntityID() == other.entity->GetEntityID();
}
