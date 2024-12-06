#pragma once

#include "Script.h"
#include <vector>

struct LapCheckpointScriptParams;
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

	/* A method for setting the parameters for every checkpoint under
	 * the manager. Can manipulate an entire track in one go this way. */
	void SetCheckpointParams(LapCheckpointScriptParams* lapCheckpointScriptParams);
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

