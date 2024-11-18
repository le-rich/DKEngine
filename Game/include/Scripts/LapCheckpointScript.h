#pragma once

#include "Script.h"
#include "LapManagerScript.h"
#include "../../../Physics/include/Aabb.h"

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
	AE86::Aabb* m_SelfBB = nullptr;
	AE86::Aabb* m_OtherBB = nullptr;
	float m_BBOffset = 0.25f;
	
	LapManagerScript* m_LapManager = nullptr;
	int m_Index;
	bool m_Registered;
};

