#include "Scripts/LapCheckpointScript.h"

LapCheckpointScript::LapCheckpointScript(Entity* mEntity) : Script(mEntity)
{
	mParams.m_Self = this->entity->transform;
}

LapCheckpointScript::~LapCheckpointScript()
{
}

void LapCheckpointScript::SetLapManager(LapManagerScript* lapManager)
{
	mParams.m_LapManager = lapManager;
}

void LapCheckpointScript::Update(float deltaTime)
{
	//if (!mParams.m_Self || !mParams.m_Other) { return; }
	//// Sets BB correctly even after setWorldPosition is used on Entity in Scene
	//if (!mParams.m_SelfBB) { mParams.m_SelfBB = new AE86::Aabb(glm::vec3(mParams.m_Self->getWorldPosition()), mParams.m_BBOffset); }

	mParams.m_OtherBB->UpdateBounds(mParams.m_Other->getWorldPosition());

	if (!mParams.m_Registered && mParams.m_SelfBB->CheckCollision(*mParams.m_OtherBB))
	{
		mParams.m_LapManager->OnCheckpointTriggered(mParams.m_Index);
		//m_OnCheckpointCallback(mParams.m_Index);
	}
}

void LapCheckpointScript::SetParameters(ScriptParams* pScriptParameters)
{
	mParams = *static_cast<LapCheckpointScriptParams*>(pScriptParameters);
	mParams.m_Self = this->entity->transform;
	mParams.m_SelfBB = new AE86::Aabb(glm::vec3(mParams.m_Self->getWorldPosition()), mParams.m_BBOffset);
	
	glm::vec3 otherPosition = glm::vec3(mParams.m_Other->getWorldPosition());
	mParams.m_OtherBB = new AE86::Aabb(otherPosition, mParams.m_BBOffset);
}
