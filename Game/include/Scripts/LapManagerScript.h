#pragma once

#include <vector>

#include "Script.h"

class LapCheckpointScript;

struct LapManagerScriptParams : ScriptParams
{
	std::vector<LapCheckpointScript*> m_Checkpoints;
	int m_CurrentLap = 1;
	int m_TotalLaps = 3;
	int m_NextCheckpointIndex = 1;
};

class LapManagerScript : public Script
{
public:
	LapManagerScript(Entity* mEntity);
	~LapManagerScript();
	
	void OnCheckpointTriggered(int checkpointIndex);

	void Update(float deltaTime) override;
	void SetParameters(ScriptParams* pScriptParameters) override;
	std::unique_ptr<Script> clone() const override {
		return std::make_unique<LapManagerScript>(*this);
	}
private:
	LapManagerScriptParams mParams{};

	void NextCheckpoint(int checkpointIndex);
	void CompleteLap();
	void CompleteRace();
};

