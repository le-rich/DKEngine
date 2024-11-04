#include "Components/ScriptComponent.h"

#include "Component.h"
#include "Entity.h"

ScriptComponent::ScriptComponent(Entity *mEntity) : Component(mEntity)
{
    this->componentType = ComponentType::Script;
}

ScriptComponent::~ScriptComponent()
{

}
