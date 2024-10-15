#ifndef TRANSFORM_H
#define TRANSFORM_H

#define GLM_FORCE_SWIZZLE
#include <glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <mutex>

/**
 * 
 */
class Transform : Component {
private:
	// parent space coords for Transform's origin's position.
	glm::vec4 localPosition;

	// parent space angular orientation
	glm::quat localOrientation;

	// parent-relative scale
	glm::vec3 localScale;

	// lock to guarantee mutual exclusion
	std::mutex mtx;

	/* 
	 * Holds a transform matrix for converting local space
	 * to world space, and world scale.
	 */
	glm::mat4 transformMatrix;
	
	// The linked entity.
	Entity* entity;

public:
	Transform(Entity* mEntity) : Component(mEntity), localPosition(0.0f, 0.0f, 0.0f, 1.0f),
		localOrientation(1.0f, 0.0f, 0.0f, 0.0f),
		localScale(1.0f, 1.0f, 1.0f),
		transformMatrix(1.0f) {};

	// the constructor, takes global position, orientation, scale, parent, child
	Transform(Entity* mEntity, glm::vec4 position, glm::quat orientation, float scale);
	~Transform();

	/** the parent might move or scale, the transform matrix needs 
	 * to then be updated. This method does that.
	 */
	void updateTransformMatrix();
	void lookAt(Transform target);


	/********************************************/
	/* SETTERS AND GETTERS FOR POS/ORIENT/SCALE */
	/********************************************/
	glm::vec4 getWorldPosition();
	glm::quat getWorldOrientation();

	glm::vec4 getLocalPosition();
	glm::quat getLocalOrientation();

	void setLocalPosition(glm::vec4 position);
	void setLocalOrientation(glm::quat orientation);

	void setWorldSpacePosition(glm::vec4 position);
	void setWorldSpaceOrientation(glm::quat orientation);

	glm::vec3 getWorldScale();
	void setWorldScale(glm::vec3 scale);

	glm::vec3 getLocalScale();
	void setLocalScale(glm::vec3 scale);

	Transform& operator=(const Transform& other);
};

#endif 