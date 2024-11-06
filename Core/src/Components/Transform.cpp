#include "Components/Transform.h"

#include "Component.h"
#include "Entity.h"

#include <glm.hpp>
#include <glm/gtc/quaternion.hpp>

Transform::Transform(Entity* mEntity) :Component(mEntity), localPosition(0.0f, 0.0f, 0.0f),
localOrientation(1.0f, 0.0f, 0.0f, 0.0f),
localScale(1.0f, 1.0f, 1.0f),
transformMatrix(1.0f)
{

}



Transform::Transform(Entity* mEntity, glm::vec4 position, glm::quat orientation, float scale)
	: Component(mEntity), localPosition(position), localOrientation(orientation), localScale(scale)
{
	updateTransformMatrix();

	// DO NOT REVERSE LINK mEntity->transform = this. This should be handled in Entity's initialization.
}

Transform::Transform(Transform const& other) : Component(other.entity), localPosition(other.localPosition),
localOrientation(other.localOrientation), localScale(other.localScale), transformMatrix(other.transformMatrix)
{}

Transform::~Transform() {}

Component* Transform::clone() {
	 return (new Transform(*this));
}

void Transform::updateTransformMatrix()
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

void Transform::lookAt(Transform* target) {}


glm::vec3 Transform::getWorldPosition()
{
    return glm::vec3(transformMatrix * glm::vec4(localPosition, 1.0));
}

glm::quat Transform::getWorldOrientation()
{
	// TODO: flesh out later when making scene graph
	return localOrientation;
}

glm::vec3 Transform::getLocalPosition()
{
	return localPosition;
}

glm::quat Transform::getLocalOrientation()
{
	return localOrientation;
}

void Transform::setLocalPosition(glm::vec3 position)
{
	localPosition.x = position.x;
	localPosition.y = position.y;
	localPosition.z = position.z;
}

void Transform::setLocalOrientation(glm::quat orientation)
{
	localOrientation.w = orientation.w;
	localOrientation.x = orientation.x;
	localOrientation.y = orientation.y;
	localOrientation.z = orientation.z;
}

void Transform::setWorldPosition(glm::vec4 position)
{
	glm::vec4 localUpdate = transformMatrix * position;

	localPosition.x = localUpdate.x;
	localPosition.y = localUpdate.y;
	localPosition.z = localUpdate.z;

    localPosition = glm::vec3(localUpdate);

}

void Transform::setWorldOrientation(glm::quat orientation)
{
    // TODO: flesh out later when making scene graph
    localOrientation.w = orientation.w;
    localOrientation.x = orientation.x;
    localOrientation.y = orientation.y;
    localOrientation.z = orientation.z;

    localOrientation = orientation;
}

glm::vec3 Transform::getWorldScale()
{
	// TODO: flesh out later when making scene graph
	return localScale;
}

void Transform::setWorldScale(glm::vec3 scale)
{
	// TODO: flesh out later when making scene graph
	localOrientation.x = scale.x;
	localOrientation.y = scale.y;
	localOrientation.z = scale.z;
}

glm::vec3 Transform::getLocalScale()
{
	return localScale;
}

void Transform::setLocalScale(glm::vec3 scale)
{
	localOrientation.x = scale.x;
	localOrientation.y = scale.y;
	localOrientation.z = scale.z;
}

Transform& Transform::operator=(const Transform& other)
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
