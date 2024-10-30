#include "Component.h"

#include "Entity.h"
#include "Components/Transform.h"

#include <Utils/IDUtils.h>

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
    return (this->componentDisplayName == other.componentDisplayName)
        &&
        (this->componentID == other.componentID);
}







