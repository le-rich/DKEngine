#include "Scripts/LapManagerScript.h"

LapManagerScript::LapManagerScript(Entity* mEntity, int laps) : Script(mEntity), m_CurrentLap(1), m_TotalLaps(laps)
{
	
}

LapManagerScript::~LapManagerScript()
{
}

void LapManagerScript::Update(float deltaTime)
{
}

void LapManagerScript::RegisterCheckpoint(LapColliderScript* checkpoint)
{
	m_LapCheckpoints.push_back(checkpoint);

	checkpoint->SetTriggerCallback([this](int checkpointID) -> bool
	{
		std::cout << "Lambda captured LapManagerScript, calling OnCheckpointTriggered." << std::endl;		
		bool isRegistered = IsCheckpointRegistered(checkpointID);
		if (!isRegistered)
		{
			int currLap = m_CurrentLap;
			OnCheckpointTriggered(checkpointID);
			if (currLap < m_CurrentLap) { return false; } // new lap started, reset checkpoint
			return true;
		}
		return false;
	});
}

bool LapManagerScript::IsCheckpointRegistered(int checkpointID)
{
	//auto it = std::find(m_TriggeredCheckpoints.begin(), m_TriggeredCheckpoints.end(), checkpointID);
	if (checkpointID >= 1 && checkpointID < m_TriggeredCheckpoints.size())
	{
		return m_TriggeredCheckpoints[checkpointID] == checkpointID;
	}

	return false;
}

void LapManagerScript::OnCheckpointTriggered(int checkpointID)
{
	std::cout << "Checkpoint " << checkpointID << " Callback Triggered" << std::endl;

	m_TriggeredCheckpoints.push_back(checkpointID);

	if (m_TriggeredCheckpoints.size() == m_LapCheckpoints.size())
	{
		std::cout << "Lap completed!" << std::endl;
		m_TriggeredCheckpoints.clear();
		++m_CurrentLap;
	}

	if (m_CurrentLap > m_TotalLaps)
	{
		std::cout << "All laps completed! Exiting Program." << std::endl;
		std::exit(0);
	}
}
