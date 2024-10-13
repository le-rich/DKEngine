#pragma once

#include "../include/Component.h";
#include <glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <mutex>

/**
 * 
 */
class Transform : public Component {
private:
	// world space coords for position.
	glm::vec4 position;

	// world space angular orientation
	glm::quat orientation;

	// scale
	float scale;

	// lock to guarantee mutual exclusion
	std::mutex mtx;

	/* 
	 * Holds a transform matrix for converting parent space
	 * to world space and vice-versa.
	 */
	glm::mat4 transformMatrix;

	// place-holder for calculations, likely there will 
	// be a better way to receive the parent transform 
	// (through the entity itself or whatever).
	Transform& parent;

public:
	// the constructor, takes global position, orientation, scale
	Transform(glm::vec3 pos, glm::quat orient, float scale);

	/** the parent might move, the transform matrix needs to then be
     * updated, alongside the position, orientation, and scale of this
	 * transform.
	 */
	void updateTransformMatrix();

	/********************************************/
	/* SETTERS AND GETTERS FOR POS/ORIENT/SCALE */
	/********************************************/
	glm::vec4 getWorldSpacePosition();
	glm::quat getWorldSpaceOrientation();

	glm::vec4 getParentSpacePosition();
	glm::vec4 getParentSpaceOrientation();

	void setParentSpacePosition(glm::vec4 position);
	void setParentSpaceOrientation(glm::quat orientation);

	void setWorldSpacePosition(glm::vec4 position);
	void setWorldSpaceOrientation(glm::quat orientation);

	float getWorldScale();
	float setWorldScale(float scale);

	float getLocalScale();
	float setLocalScale(float scale);
};
