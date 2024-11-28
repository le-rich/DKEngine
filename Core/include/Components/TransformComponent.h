#pragma once
#define GLM_FORCE_SWIZZLE

#include "Component.h"

#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <mutex>

class Entity;

struct Transform
{
    // parent space coords for Transform's origin's position.
    glm::vec3 localPosition;

	// parent space angular orientation
	glm::quat localOrientation;

    // parent-relative scale
    glm::vec3 localScale;
};

/**
 * Transform Component
 */
class TransformComponent : public Component
{
private:
    Transform mTransform;

    // Holds a transform matrix for converting local space to world space, and world scale.
    glm::mat4 transformMatrix;

public:
    TransformComponent(Entity* mEntity);
    // the constructor, takes global position, orientation, scale, parent, child
    TransformComponent(Entity* mEntity, glm::vec4 position, glm::quat orientation, float scale);
    TransformComponent(Entity* mEntity, Transform transform);
    ~TransformComponent();

	// lock to guarantee mutual exclusion
	std::mutex mtx;

    void lookAt(TransformComponent* target);


    /********************************************/
    /* SETTERS AND GETTERS FOR POS/ORIENT/SCALE */
    /********************************************/

    // returns a 4x4 transformation matrix local to the parent
    const glm::mat4 getLocalTransformMatrix();

    // returns a world transformation matrix
    const glm::mat4 getTransformMatrix();
    inline const void setTransform(Transform& pTransform) { mTransform = pTransform; }

	glm::vec3 getWorldPosition();
	glm::quat getWorldOrientation() const;

	glm::vec3 getLocalPosition();
	glm::quat getLocalOrientation() const;

	void setLocalPosition(glm::vec3 position);
	void setLocalOrientation(glm::quat orientation);

	void setWorldPosition(glm::vec4 position);
	void setWorldOrientation(glm::quat orientation);

	glm::vec3 getWorldScale() const;
	void setWorldScale(glm::vec3 scale);

	glm::vec3 getLocalScale() const;
	void setLocalScale(glm::vec3 scale);

    glm::vec3 getForward();
    glm::vec3 getBack();
    glm::vec3 getRight();
    glm::vec3 getLeft();
    glm::vec3 getUp();

    TransformComponent& operator=(const TransformComponent& other);

	// copy constructor for clone
	TransformComponent(const TransformComponent& other);
	Component* clone() const override;
};
