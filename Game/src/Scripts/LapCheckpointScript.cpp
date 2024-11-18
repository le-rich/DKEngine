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
	this->m_OtherBB = SetBB(m_Other->getWorldPosition());
}

void LapCheckpointScript::Update(float deltaTime)
{
	if (!m_Self || !m_Other) { return; }
	// Sets BB correctly even after setWorldPosition is used on Entity in Scene
	if (!m_SelfBB) { m_SelfBB = SetBB(m_Self->getWorldPosition()); }

	UpdateBB(m_Other->getWorldPosition(), m_OtherBB);

	if (!m_Registered && AABBCollision(m_SelfBB, m_OtherBB))
	{
		m_LapManager->OnCheckpointTriggered(m_Index);
	}
}

bool LapCheckpointScript::AABBCollision(AABB* self, AABB* other)
{
	float d1x = other->min.x - self->max.x;
	float d1y = other->min.y - self->max.y;
	float d1z = other->min.z - self->max.z;
	float d2x = self->min.x - other->max.x;
	float d2y = self->min.y - other->max.y;
	float d2z = self->min.z - other->max.z;

	if (d1x > 0.0f || d1y > 0.0f || d1z > 0.0f)
		return false;
	if (d2x > 0.0f || d2y > 0.0f || d2z > 0.0f)
		return false;

	return true;
}

AABB* LapCheckpointScript::SetBB(const glm::vec3 position)
{
	glm::vec3 boxMin = glm::vec3(position.x - m_BBOffset, position.y - m_BBOffset, position.z - m_BBOffset);
	glm::vec3 boxMax = glm::vec3(position.x + m_BBOffset, position.y + m_BBOffset, position.z + m_BBOffset);
	return new AABB(boxMin, boxMax);
}

void LapCheckpointScript::UpdateBB(const glm::vec3 position, AABB* BB) const
{
	BB->min.x = position.x - m_BBOffset;
	BB->min.y = position.y - m_BBOffset;
	BB->min.z = position.z - m_BBOffset;
	BB->max.x = position.x + m_BBOffset;
	BB->max.y = position.y + m_BBOffset;
	BB->max.z = position.z + m_BBOffset;
}
