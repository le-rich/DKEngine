#pragma once

#include <functional>

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
	static int ID;

	LapColliderScript(Entity* mEntity);
	~LapColliderScript();

	void Update(float deltaTime) override;

	//void SetSelf(TransformComponent* transform) { m_Self = transform; }
	void SetOther(TransformComponent* transform) { m_Other = transform; }
	bool AABBCollision(AABB* self, AABB* other);

	using TriggerCallback = std::function<bool(int)>;
	void SetTriggerCallback(TriggerCallback callback);
	void ResetRegistry() { m_IsCheckpointRegistered = false; }

	std::unique_ptr<Script> clone() const override {
		return std::make_unique<LapColliderScript>(*this);
	}
private:
	TransformComponent* m_Self = nullptr;
	TransformComponent* m_Other = nullptr;
	AABB* m_SelfBox = nullptr;
	AABB* m_OtherBox = nullptr;
	float m_BoxOffset = 0.25f;
	int m_ID;
	TriggerCallback m_OnTrigger;
	bool m_IsCheckpointRegistered;
	bool m_IsAlreadyOverlapping;
};

