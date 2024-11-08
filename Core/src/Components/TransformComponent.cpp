#include "Components/TransformComponent.h"

#include "Component.h"
#include "Entity.h"

#include <glm.hpp>
#include <glm/gtc/quaternion.hpp>

TransformComponent::TransformComponent(Entity* mEntity) :Component(mEntity), localPosition(0.0f, 0.0f, 0.0f),
	localOrientation(1.0f, 0.0f, 0.0f, 0.0f),
	localScale(1.0f, 1.0f, 1.0f),
	transformMatrix(1.0f) 
{

}



TransformComponent::TransformComponent(Entity* mEntity, glm::vec4 position, glm::quat orientation, float scale)
	: Component(mEntity), localPosition(position), localOrientation(orientation), localScale(scale)
{
    updateTransformMatrix();

    // DO NOT REVERSE LINK mEntity->transform = this. This should be handled in Entity's initialization.
}

TransformComponent::~TransformComponent()
{
}

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

        transformMatrix = parentTransformMatrix *
            // scale, rotate, translate matrix for parent: 
            (
                // creates a translation matrix
                glm::translate(glm::mat4(1.0f), parentLocationXYZ) *
                glm::mat4_cast(parentOrientation) * // gets a rotation matrix from the quaternion
                glm::mat4( // creates a scale matrix 
                    glm::vec4(entity->getParent()->transform->localScale.x, 0.0f, 0.0f, 0.0f),
                    glm::vec4(0.0f, entity->getParent()->transform->localScale.y, 0.0f, 0.0f),
                    glm::vec4(0.0f, 0.0f, entity->getParent()->transform->localScale.z, 0.0f),
                    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
                )
                );
    }
}

void TransformComponent::lookAt(TransformComponent* target)
{

}


glm::vec3 TransformComponent::getWorldPosition()
{
    return glm::vec3(transformMatrix * glm::vec4(localPosition, 1.0));
}

glm::quat TransformComponent::getWorldOrientation() const {
	// TODO: flesh out later when making scene graph
	return localOrientation;
}

glm::vec3 TransformComponent::getLocalPosition()
{
    return localPosition;
}

glm::quat TransformComponent::getLocalOrientation() const {
	return localOrientation;
}

void TransformComponent::setLocalPosition(glm::vec3 position)
{
    localPosition.x = position.x;
    localPosition.y = position.y;
    localPosition.z = position.z;
}

void TransformComponent::setLocalOrientation(glm::quat orientation) {
	localOrientation.w = orientation.w;
	localOrientation.x = orientation.x;
	localOrientation.y = orientation.y;
	localOrientation.z = orientation.z;
}

void TransformComponent::setWorldPosition(glm::vec4 position)
{
    glm::vec4 localUpdate = transformMatrix * position;

    localPosition.x = localUpdate.x;
    localPosition.y = localUpdate.y;
    localPosition.z = localUpdate.z;

    localPosition = glm::vec3(localUpdate);

}

void TransformComponent::setWorldOrientation(glm::quat orientation)
{
    // TODO: flesh out later when making scene graph
    localOrientation.w = orientation.w;
    localOrientation.x = orientation.x;
    localOrientation.y = orientation.y;
    localOrientation.z = orientation.z;

    localOrientation = orientation;
}

glm::vec3 TransformComponent::getWorldScale() const {
	// TODO: flesh out later when making scene graph
	return localScale;
}

void TransformComponent::setWorldScale(glm::vec3 scale) {
	// TODO: flesh out later when making scene graph
	localOrientation.x = scale.x;
	localOrientation.y = scale.y;
	localOrientation.z = scale.z;
}

glm::vec3 TransformComponent::getLocalScale() const {
	return localScale;
}

void TransformComponent::setLocalScale(glm::vec3 scale) {
	localOrientation.x = scale.x;
	localOrientation.y = scale.y;
	localOrientation.z = scale.z;
}

TransformComponent& TransformComponent::operator=(const TransformComponent& other)
{
    if (this == &other)
    {
        return *this;
    }

    this->localPosition = other.localPosition;
    this->localOrientation = other.localOrientation;
    this->localScale = other.localScale;

    // DO NOT copy mutex.

    this->transformMatrix = other.transformMatrix;
    this->entity->GetEntityID() == other.entity->GetEntityID();
}

TransformComponent::TransformComponent(const TransformComponent& other)
    : Component(other.entity), 
    localPosition(other.localPosition),
    localOrientation(other.localOrientation),
    localScale(other.localScale),
    transformMatrix(other.transformMatrix)
{}

Component* TransformComponent::clone() const {
    return new TransformComponent(*this);
}