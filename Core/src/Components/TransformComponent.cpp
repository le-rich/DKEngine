#include "Components/TransformComponent.h"

#include "Component.h"
#include "Entity.h"

#include <glm.hpp>
#include <glm/gtc/quaternion.hpp>

void TransformComponent::UpdateTransfomMatrix()
{
	auto parent = this->entity->getParent();
	if (parent)
	{
		transformMatrix = parent->transform->getTransformMatrix();
	}
}

TransformComponent::TransformComponent(Entity* mEntity) : Component(mEntity), mTransform{ glm::vec3(0.0f, 0.0f, 0.0f) , glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.f) }, transformMatrix(glm::mat4(1.f))
{}

TransformComponent::TransformComponent(Entity* mEntity, glm::vec4 position, glm::quat orientation, float scale)
	: Component(mEntity), mTransform{ position, orientation, glm::vec3(scale) }, transformMatrix(glm::mat4(1.f))
{

	// DO NOT REVERSE LINK mEntity->transform = this. This should be handled in Entity's initialization.
}

TransformComponent::TransformComponent(Entity* mEntity, Transform transform) : Component(mEntity), mTransform(transform), transformMatrix(glm::mat4(1.f))
{}

TransformComponent::~TransformComponent()
{}

const glm::mat4 TransformComponent::getLocalTransformMatrix()
{
	// right-to-left linear transformations: scale, rotate, translate    
	return
		glm::translate(glm::mat4(1.0f), mTransform.localPosition) *
		glm::mat4_cast(mTransform.localOrientation) *
		glm::mat4(
			glm::vec4(mTransform.localScale.x, 0.0f, 0.0f, 0.0f),
			glm::vec4(0.0f, mTransform.localScale.y, 0.0f, 0.0f),
			glm::vec4(0.0f, 0.0f, mTransform.localScale.z, 0.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
		);
}

const glm::mat4 TransformComponent::getTransformMatrix()
{
	UpdateTransfomMatrix();
	return transformMatrix * getLocalTransformMatrix();
}

void TransformComponent::lookAt(TransformComponent* target)
{
}


glm::vec3 TransformComponent::getWorldPosition()
{
	UpdateTransfomMatrix();
	return glm::vec3(transformMatrix * glm::vec4(mTransform.localPosition, 1.0));
}

glm::quat TransformComponent::getWorldOrientation()
{
	UpdateTransfomMatrix();
	glm::mat4 worldTransform = transformMatrix * getLocalTransformMatrix();
	glm::vec3 scale;
	scale.x = glm::length(glm::vec3(worldTransform[0])); // Basis vector X
	scale.y = glm::length(glm::vec3(worldTransform[1])); // Basis vector Y
	scale.z = glm::length(glm::vec3(worldTransform[2])); // Basis vector Z

	const glm::mat3 rotMtx(
		glm::vec3(worldTransform[0]) / scale[0],
		glm::vec3(worldTransform[1]) / scale[1],
		glm::vec3(worldTransform[2]) / scale[2]);

	return glm::quat_cast(rotMtx);
}

glm::vec3 TransformComponent::getLocalPosition() const
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
	UpdateTransfomMatrix();
	glm::vec4 localUpdate = position * glm::inverse(transformMatrix);
	mTransform.localPosition = glm::vec3(localUpdate);

}

void TransformComponent::setWorldOrientation(glm::quat orientation)
{
	// TODO: flesh out later when making scene graph
	mTransform.localOrientation = orientation;
}

glm::vec3 TransformComponent::getWorldScale()
{
	glm::mat4 transfrom = getTransformMatrix();
	float scaleX = glm::length(glm::vec3(transfrom[0]));
	float scaleY = glm::length(glm::vec3(transfrom[1]));
	float scaleZ = glm::length(glm::vec3(transfrom[2]));

	return glm::vec3(scaleX, scaleY, scaleZ);
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

glm::vec3 TransformComponent::getForward() {
	return mTransform.localOrientation * glm::vec3(0, 0, 1);
}

glm::vec3 TransformComponent::getBack() {
	return mTransform.localOrientation * glm::vec3(0, 0, -1);
}


glm::vec3 TransformComponent::getRight() {
	return mTransform.localOrientation * glm::vec3(-1, 0, 0);
}

glm::vec3 TransformComponent::getLeft() {
	return mTransform.localOrientation * glm::vec3(1, 0, 0);
}

glm::vec3 TransformComponent::getUp() {
	return mTransform.localOrientation * glm::vec3(0, 1, 0);
}

TransformComponent& TransformComponent::operator=(const TransformComponent& other)
{
	this->mTransform.localPosition = other.mTransform.localPosition;
	this->mTransform.localOrientation = other.mTransform.localOrientation;
	this->mTransform.localScale = other.mTransform.localScale;

	// DO NOT copy mutex.
	this->transformMatrix = other.transformMatrix;
	this->entity->GetEntityID() == other.entity->GetEntityID();
	return *this;
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
