#pragma once

#include "Script.h"
#include "Components/TransformComponent.h"

struct AABB
{
	glm::vec3 min;
	glm::vec3 max;

	AABB(glm::vec3 min, glm::vec3 max) : min(min), max(max) {}
};

class LapColliderScript : public Script
{
public:
	LapColliderScript(Entity* mEntity);
	~LapColliderScript();

	void Update(float deltaTime) override;

	void SetSelf(TransformComponent* transform) { m_Self = transform; }
	void SetOther(TransformComponent* transform) { m_Other = transform; }
	bool AABBCollision(AABB* self, AABB* other);

	std::unique_ptr<Script> clone() const override {
		return std::make_unique<LapColliderScript>(*this);
	}
private:
	TransformComponent* m_Self = nullptr;
	TransformComponent* m_Other = nullptr;
	AABB* m_SelfBox;
	AABB* m_OtherBox;
};

