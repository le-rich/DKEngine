#ifndef TRANSFORM_H
#define TRANSFORM_H

#define GLM_FORCE_SWIZZLE
#include <glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <mutex>

/**
 * 
 */
class Transform {
private:
	// parent space coords for Transform's origin's position.
	glm::vec3 localPosition;

	// parent space angular orientation
	glm::quat localOrientation;

	// parent-relative scale
	glm::vec3 localScale;

	/* 
	 * Holds a transform matrix for converting local space
	 * to world space, and world scale.
	 */
	glm::mat4 transformMatrix;

	// place-holder for calculations, likely there will 
	// be a better way to receive the parent transform 
	// (through the entity itself or whatever).
	Transform* parent;
	Transform* child;

public:
	// the constructor, takes global position, orientation, scale, parent, child
	Transform(glm::vec4 position, glm::quat orientation, float scale, Transform* parent, Transform* child);

	Transform() : localPosition(glm::vec3(0.0f)), localOrientation(glm::quat(glm::vec3(0.0f, 0.0f, 0.0f))), localScale(1.0f), parent(nullptr), child(nullptr), transformMatrix(glm::mat4(1.0f)) {}

	// lock to guarantee mutual exclusion
	std::mutex mtx;


	/** the parent might move or scale, the transform matrix needs 
	 * to then be updated. This method does that.
	 */
	void updateTransformMatrix();


	/********************************************/
	/* SETTERS AND GETTERS FOR POS/ORIENT/SCALE */
	/********************************************/
	inline const glm::mat4 getTransformMatrix() { return transformMatrix; }

	glm::vec3 getWorldPosition();
	glm::quat getWorldOrientation();

	glm::vec3 getLocalPosition();
	glm::quat getLocalOrientation();

	void setLocalPosition(glm::vec3 position);
	void setLocalOrientation(glm::quat orientation);

	void setWorldSpacePosition(glm::vec4 position);
	void setWorldSpaceOrientation(glm::quat orientation);

	glm::vec3 getWorldScale();
	void setWorldScale(glm::vec3 scale);

	glm::vec3 getLocalScale();
	void setLocalScale(glm::vec3 scale);
};

#endif 