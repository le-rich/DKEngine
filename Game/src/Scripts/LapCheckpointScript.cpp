#include "Scripts/LapCheckpointScript.h"

LapCheckpointScript::LapCheckpointScript(Entity* mEntity) : Script(mEntity), m_Index(0), m_Registered(false)
{
	m_Self = this->entity->transform;
}

LapCheckpointScript::~LapCheckpointScript()
{
}

void LapCheckpointScript::Init(LapManagerScript* lapManager, TransformComponent* other, int checkpointIndex)
{
	this->m_LapManager = lapManager;
	this->m_Other = other;
	this->m_Index = checkpointIndex;
	glm::vec3 otherPosition = glm::vec3(this->m_Other->getWorldPosition());
	this->m_OtherBB = new AE86::Aabb(otherPosition, m_BBOffset);
}

void LapCheckpointScript::Update(float deltaTime)
{
	if (!m_Self || !m_Other) { return; }
	// Sets BB correctly even after setWorldPosition is used on Entity in Scene
	if (!m_SelfBB) { m_SelfBB = new AE86::Aabb(glm::vec3(this->m_Self->getWorldPosition()), m_BBOffset); }

	m_OtherBB->UpdateBounds(m_Other->getWorldPosition());

	if (!m_Registered && m_SelfBB->CheckCollision(*m_OtherBB))
	{
		m_LapManager->OnCheckpointTriggered(m_Index);
	}
}
