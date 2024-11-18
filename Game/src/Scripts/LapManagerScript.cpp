#include "Scripts/LapManagerScript.h"
#include "Scripts/LapCheckpointScript.h"

LapManagerScript::LapManagerScript(Entity* mEntity, int laps) 
	: Script(mEntity), m_CurrentLap(1), m_TotalLaps(laps), m_NextCheckpointIndex(1)
{
}

LapManagerScript::~LapManagerScript()
{
}

void LapManagerScript::OnCheckpointTriggered(int checkpointIndex)
{
	if (checkpointIndex == m_NextCheckpointIndex)
	{
		NextCheckpoint(checkpointIndex);
		if (m_NextCheckpointIndex > m_Checkpoints.size()) { CompleteLap(); }
		if (m_CurrentLap > m_TotalLaps) { CompleteRace(); }
	}
}

void LapManagerScript::Update(float deltaTime)
{
}

void LapManagerScript::NextCheckpoint(int checkpointIndex)
{
	m_Checkpoints[checkpointIndex - 1]->SetCheckpointRegistered(true);
	std::cout << "Hit Checkpoint: " << checkpointIndex << std::endl;
	m_NextCheckpointIndex++;
}

void LapManagerScript::CompleteLap()
{
	std::cout << "Lap " << m_CurrentLap << " of " << m_TotalLaps << " completed!" << std::endl;
	++m_CurrentLap;
	m_NextCheckpointIndex = 1;
	for (const auto& checkpoint : m_Checkpoints) { checkpoint->SetCheckpointRegistered(false); }
}

void LapManagerScript::CompleteRace()
{
	// TODO: Go back to car selection after race completion
	std::cout << "All laps completed! Exiting Program." << std::endl;
	std::exit(0);
}
