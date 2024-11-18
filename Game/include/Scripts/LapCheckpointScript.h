#pragma once

#include "Script.h"
#include "LapManagerScript.h"

struct AABB
{
	glm::vec3 min;
	glm::vec3 max;

	AABB(glm::vec3 min, glm::vec3 max) : min(min), max(max) {}
};

class LapCheckpointScript : public Script
{
public:
	LapCheckpointScript(Entity* mEntity);
	~LapCheckpointScript();

	void Init(LapManagerScript* lapManager, TransformComponent* other, int checkpointIndex);
	bool GetCheckpointRegistered() const { return m_Registered; }
	void SetCheckpointRegistered(bool isRegistered) { m_Registered = isRegistered; }

	void Update(float deltaTime) override;
	std::unique_ptr<Script> clone() const override {
		return std::make_unique<LapCheckpointScript>(*this);
	}
private:
	TransformComponent* m_Self = nullptr;
	TransformComponent* m_Other = nullptr;
	AABB* m_SelfBB = nullptr;
	AABB* m_OtherBB = nullptr;
	float m_BBOffset = 0.25f;
	
	LapManagerScript* m_LapManager = nullptr;
	int m_Index;
	bool m_Registered;

	bool AABBCollision(AABB* self, AABB* other);
	AABB* SetBB(const glm::vec3 position);
	void UpdateBB(const glm::vec3 position, AABB* BB) const;
};

