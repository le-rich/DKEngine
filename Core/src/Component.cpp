#include "Component.h"

Component::Component(Entity* mEntity)
{
	this->componentID = uuidGen.getUUID();
	this->entity = mEntity;
}

Component::~Component()
{

}

bool Component::operator==(const Component& other) const
{
	return componentID == other.componentID;
}

