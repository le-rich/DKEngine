#include "Components/ScriptComponent.h"

#include "Component.h"
#include "Entity.h"

ScriptComponent::ScriptComponent(Entity* entity) : Component(entity)
{
	this->componentType = ComponentType::Script;
}

ScriptComponent::~ScriptComponent()
{

}

Component* ScriptComponent::clone() const
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

// copy constructor
ScriptComponent::ScriptComponent(const ScriptComponent& other)
    : Component(other.entity) {
    this->componentType = ComponentType::Script;

    // Deep copy each script using clone()
    for (const auto& script : other.scripts) {
        scripts.push_back(script->clone());
    }
}
