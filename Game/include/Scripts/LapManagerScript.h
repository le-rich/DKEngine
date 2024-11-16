#pragma once

#include <vector>

#include "LapColliderScript.h"

class LapManagerScript : public Script
{
public:
	LapManagerScript(Entity* mEntity, int laps);
	~LapManagerScript();

	void Update(float deltaTime) override;

	void RegisterCheckpoint(LapColliderScript* checkpoint);
	bool IsCheckpointRegistered(int checkpointID);
	void OnCheckpointTriggered(int checkpointID);

	std::unique_ptr<Script> clone() const override {
		return std::make_unique<LapManagerScript>(*this);
	}
private:
	std::vector<LapColliderScript*> m_LapCheckpoints;
	std::vector<int> m_TriggeredCheckpoints;
	int m_CurrentLap;
	int m_TotalLaps;
};

