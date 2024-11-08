#include "Entity.h"

Entity::Entity(const Entity& other)
{
    entityID = other.entityID;
    entityDisplayName = other.entityDisplayName;
    transform = (TransformComponent*)other.transform->clone();

    // TODO
    components.reserve(other.components.size());
    for (auto* elem : other.components)
    {
        components.push_back(elem->clone());
    }


    // TODO
    children.reserve(other.children.size());
    for (auto* elem : other.children)
    {
        Entity* child = new Entity(*elem);
        child->setParent(this);
        children.push_back(child);
    }

}
