#include "Scripts/LapManagerScript.h"
#include "Scripts/LapCheckpointScript.h"

LapManagerScript::LapManagerScript(Entity* mEntity) : Script(mEntity)
{
}

LapManagerScript::~LapManagerScript()
{
}

void LapManagerScript::OnCheckpointTriggered(int checkpointIndex)
{
	if (checkpointIndex == mParams.m_NextCheckpointIndex)
	{
		this->NextCheckpoint(checkpointIndex);
		if (mParams.m_NextCheckpointIndex > mParams.m_Checkpoints.size()) { CompleteLap(); }
		if (mParams.m_CurrentLap > mParams.m_TotalLaps) { CompleteRace(); }
	}
}

void LapManagerScript::Update(float deltaTime)
{
}

void LapManagerScript::SetCheckpointParams(ScriptParams* lapCheckpointScriptParams) {
	for (LapCheckpointScript* lapCheckpointScript : mParams.m_Checkpoints) {
		lapCheckpointScript->SetParameters(lapCheckpointScriptParams);
	}
}

void LapManagerScript::SetParameters(ScriptParams* pScriptParameters)
{
	mParams = *static_cast<LapManagerScriptParams*>(pScriptParameters);
}

void LapManagerScript::NextCheckpoint(int checkpointIndex)
{
	mParams.m_Checkpoints[checkpointIndex - 1]->SetCheckpointRegistered(true);
	std::cout << "Hit Checkpoint: " << checkpointIndex << std::endl;
	mParams.m_NextCheckpointIndex++;
}

void LapManagerScript::CompleteLap()
{
	std::cout << "Lap " << mParams.m_CurrentLap << " of " << mParams.m_TotalLaps << " completed!" << std::endl;
	++mParams.m_CurrentLap;
	mParams.m_NextCheckpointIndex = 1;
	for (const auto& checkpoint : mParams.m_Checkpoints) { checkpoint->SetCheckpointRegistered(false); }
}

void LapManagerScript::CompleteRace()
{
	// TODO: Go back to car selection after race completion
	std::cout << "All laps completed!" << std::endl;
}
