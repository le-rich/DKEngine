#pragma once

#include <vector>

#include "Script.h"

class LapCheckpointScript;

class LapManagerScript : public Script
{
public:
	LapManagerScript(Entity* mEntity, int laps);
	~LapManagerScript();
	
	void SetCheckpoints(std::vector<LapCheckpointScript*> checkpoints) { m_Checkpoints = checkpoints; }
	void OnCheckpointTriggered(int checkpointIndex);

	void Update(float deltaTime) override;
	std::unique_ptr<Script> clone() const override {
		return std::make_unique<LapManagerScript>(*this);
	}
private:
	std::vector<LapCheckpointScript*> m_Checkpoints;
	int m_CurrentLap;
	int m_TotalLaps;
	int m_NextCheckpointIndex;

	void NextCheckpoint(int checkpointIndex);
	void CompleteLap();
	void CompleteRace();
};

