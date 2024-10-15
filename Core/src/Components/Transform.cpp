#include "Components/Transform.h"
#include<glm/gtc/quaternion.hpp>


Transform::Transform(glm::vec4 position, glm::quat orientation, float scale, Transform* parent, Transform* child)
	: localPosition(position), localOrientation(orientation), localScale(scale), parent(parent), child(child) 
{
	updateTransformMatrix();
}

void Transform::updateTransformMatrix() {
	if (parent == nullptr) {
		transformMatrix = glm::mat4(1.0f); // identity matrix
	}
	else {
		transformMatrix =
			// grand parent transform matrix
			parent->transformMatrix *
			// scale, rotate, translate matrix for parent: 
			(
				// creates a translation matrix
				glm::translate(glm::mat4(1.0f), parent->localPosition.xyz()) *
				glm::mat4_cast(parent->localOrientation) * // gets a rotation matrix from the quaternion
				glm::mat4( // creates a scale matrix 
					glm::vec4(parent->localScale.x, 0.0f, 0.0f, 0.0f),
					glm::vec4(0.0f, parent->localScale.y, 0.0f, 0.0f),
					glm::vec4(0.0f, 0.0f, parent->localScale.z, 0.0f),
					glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
				)
			);
	}
}


glm::vec4 Transform::getWorldPosition() {
	return transformMatrix * localPosition;
}

glm::quat Transform::getWorldOrientation() {
	// TODO: flesh out later when making scene graph
	return localOrientation;
}

glm::vec4 Transform::getLocalPosition() {
	return localPosition;
}

glm::quat Transform::getLocalOrientation() {
	return localOrientation;
}

void Transform::setLocalPosition(glm::vec4 position) {
	localPosition.x = position.x;
	localPosition.y = position.y;
	localPosition.z = position.z;
}

void Transform::setLocalOrientation(glm::quat orientation) {
	localOrientation.w = orientation.w;
	localOrientation.x = orientation.x;
	localOrientation.y = orientation.y;
	localOrientation.z = orientation.z;
}

void Transform::setWorldSpacePosition(glm::vec4 position) {
	glm::vec4 localUpdate = transformMatrix * position;

	localPosition.x = localUpdate.x;
	localPosition.y = localUpdate.y;
	localPosition.z = localUpdate.z;

}

void Transform::setWorldSpaceOrientation(glm::quat orientation) {
	// TODO: flesh out later when making scene graph
	localOrientation.w = orientation.w;
	localOrientation.x = orientation.x;
	localOrientation.y = orientation.y;
	localOrientation.z = orientation.z;
}

glm::vec3 Transform::getWorldScale() {
	// TODO: flesh out later when making scene graph
	return localScale;
}

void Transform::setWorldScale(glm::vec3 scale) {
	// TODO: flesh out later when making scene graph
	localOrientation.x = scale.x;
	localOrientation.y = scale.y;
	localOrientation.z = scale.z;
}

glm::vec3 Transform::getLocalScale() {
	return localScale;
}

void Transform::setLocalScale(glm::vec3 scale) {
	localOrientation.x = scale.x;
	localOrientation.y = scale.y;
	localOrientation.z = scale.z;
}
