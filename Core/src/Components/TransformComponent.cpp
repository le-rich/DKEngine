#include "Components/TransformComponent.h"

#include "Component.h"
#include "Entity.h"

#include <glm.hpp>
#include <glm/gtc/quaternion.hpp>

TransformComponent::TransformComponent(Entity* mEntity) : Component(mEntity), mTransform{ glm::vec3(0.0f, 0.0f, 0.0f) , glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.f) },
transformMatrix(1.0f)
{
}

TransformComponent::TransformComponent(Entity* mEntity, glm::vec4 position, glm::quat orientation, float scale)
    : Component(mEntity), mTransform{ position, orientation, glm::vec3(scale) }
{
    updateTransformMatrix();

    // DO NOT REVERSE LINK mEntity->transform = this. This should be handled in Entity's initialization.
}

TransformComponent::TransformComponent(Entity* mEntity, Transform transform) : Component(mEntity), mTransform(transform)
{
    updateTransformMatrix();
}

TransformComponent::~TransformComponent()
{}

void TransformComponent::updateTransformMatrix()
{
    if (entity->getParent() == nullptr)
    {
        transformMatrix = glm::mat4(1.0f); // identity matrix
    }
    else
    {
        glm::mat4 parentTransformMatrix = entity->getParent()->transform->getTransformMatrix();
        glm::vec3 parentLocationXYZ = entity->getParent()->transform->getLocalPosition().xyz;
        glm::quat parentOrientation = entity->getParent()->transform->getLocalOrientation();
        glm::vec3 parentScale = entity->getParent()->transform->getLocalScale();

        transformMatrix = parentTransformMatrix *
            // scale, rotate, translate matrix for parent: 
            (
                // creates a translation matrix
                glm::translate(glm::mat4(1.0f), parentLocationXYZ) *
                glm::mat4_cast(parentOrientation) * // gets a rotation matrix from the quaternion
                glm::scale( glm::mat4(1.f), parentScale)
                );
    }
}

void TransformComponent::lookAt(TransformComponent* target)
{

}


glm::vec3 TransformComponent::getWorldPosition()
{
    //updateTransformMatrix();
    return glm::vec3(transformMatrix * glm::vec4(mTransform.localPosition, 1.0));
}

glm::quat TransformComponent::getWorldOrientation() const
{
    // TODO: flesh out later when making scene graph
    return mTransform.localOrientation;
}

glm::vec3 TransformComponent::getLocalPosition()
{
    return mTransform.localPosition;
}

glm::quat TransformComponent::getLocalOrientation() const
{
    return mTransform.localOrientation;
}

void TransformComponent::setLocalPosition(glm::vec3 position)
{
    mTransform.localPosition.x = position.x;
    mTransform.localPosition.y = position.y;
    mTransform.localPosition.z = position.z;
}

void TransformComponent::setLocalOrientation(glm::quat orientation)
{
    mTransform.localOrientation.w = orientation.w;
    mTransform.localOrientation.x = orientation.x;
    mTransform.localOrientation.y = orientation.y;
    mTransform.localOrientation.z = orientation.z;
}

void TransformComponent::setWorldPosition(glm::vec4 position)
{
    glm::vec4 localUpdate = transformMatrix * position;
    mTransform.localPosition = glm::vec3(localUpdate);

}

void TransformComponent::setWorldOrientation(glm::quat orientation)
{
    // TODO: flesh out later when making scene graph
    mTransform.localOrientation = orientation;
}

glm::vec3 TransformComponent::getWorldScale() const
{
    // TODO: flesh out later when making scene graph
    return mTransform.localScale;
}

void TransformComponent::setWorldScale(glm::vec3 scale)
{
    // TODO: flesh out later when making scene graph
    mTransform.localOrientation.x = scale.x;
    mTransform.localOrientation.y = scale.y;
    mTransform.localOrientation.z = scale.z;
}

glm::vec3 TransformComponent::getLocalScale() const
{
    return mTransform.localScale;
}

void TransformComponent::setLocalScale(glm::vec3 scale)
{
    mTransform.localScale.x = scale.x;
    mTransform.localScale.y = scale.y;
    mTransform.localScale.z = scale.z;
}

TransformComponent& TransformComponent::operator=(const TransformComponent& other)
{
    if (this == &other)
    {
        return *this;
    }

    this->mTransform.localPosition = other.mTransform.localPosition;
    this->mTransform.localOrientation = other.mTransform.localOrientation;
    this->mTransform.localScale = other.mTransform.localScale;

    // DO NOT copy mutex.

    this->transformMatrix = other.transformMatrix;
    this->entity->GetEntityID() == other.entity->GetEntityID();
}

TransformComponent::TransformComponent(const TransformComponent& other)
    : Component(other.entity),
    mTransform(other.mTransform),
    transformMatrix(other.transformMatrix)
{}

Component* TransformComponent::clone() const
{
    return new TransformComponent(*this);
}