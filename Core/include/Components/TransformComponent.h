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

    /** the parent might move or scale, the transform matrix needs
     * to then be updated. This method does that.
     */
    void updateTransformMatrix();
    void lookAt(TransformComponent* target);


    /********************************************/
    /* SETTERS AND GETTERS FOR POS/ORIENT/SCALE */
    /********************************************/
    inline const glm::mat4 getTransformMatrix() { return transformMatrix; }
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

    TransformComponent& operator=(const TransformComponent& other);

    // copy constructor for clone
    TransformComponent(const TransformComponent& other);
    Component* clone() const override;
};
