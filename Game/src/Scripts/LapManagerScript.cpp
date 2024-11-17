#include "Scripts/LapManagerScript.h"

LapManagerScript::LapManagerScript(Entity* mEntity, int laps) 
	: Script(mEntity), m_CurrentLap(1), m_TotalLaps(laps), m_NextCheckpointIndex(0)
{
}

LapManagerScript::~LapManagerScript()
{
}

void LapManagerScript::OnCheckpointTriggered(int checkpointIndex)
{
}

void LapManagerScript::Update(float deltaTime)
{
}
