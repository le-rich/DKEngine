#include <iostream>
#include <string>

#include "Scripts/LapColliderScript.h"

LapColliderScript::LapColliderScript(Entity* mEntity) : Script(mEntity)
{
	m_Self = this->entity->transform;
	m_Other = nullptr;

}

LapColliderScript::~LapColliderScript() {}

void LapColliderScript::Update(float deltaTime)
{
	if (!m_Self || !m_Other) { return; }

	glm::vec3 selfPosition = m_Self->getWorldPosition();
	glm::vec3 otherPosition = m_Other->getWorldPosition();

	// make bounding box for this entity
	glm::vec3 boxMin = glm::vec3(selfPosition.x - m_BoxOffset, selfPosition.y - m_BoxOffset, selfPosition.z - m_BoxOffset);
	glm::vec3 boxMax = glm::vec3(selfPosition.x + m_BoxOffset, selfPosition.y + m_BoxOffset, selfPosition.z + m_BoxOffset);
	m_SelfBox = new AABB(boxMin, boxMax);

	// make bounding box for car entity
	boxMin = glm::vec3(otherPosition.x - m_BoxOffset, otherPosition.y - m_BoxOffset, otherPosition.z - m_BoxOffset);
	boxMax = glm::vec3(otherPosition.x + m_BoxOffset, otherPosition.y + m_BoxOffset, otherPosition.z + m_BoxOffset);
	m_OtherBox = new AABB(boxMin, boxMax);

	if (AABBCollision(m_SelfBox, m_OtherBox))
	{
		std::string name = this->entity->GetDisplayName();
		std::cout << "Overlap with " << name << " detected!!!" << std::endl;
	}
	else
	{
		std::cout << "No Overlap" << std::endl;
	}
}

bool LapColliderScript::AABBCollision(AABB* self, AABB* other)
{
	
	float d1x = other->min.x - self->max.x;
	float d1y = other->min.y - self->max.y;
	float d2x = self->min.x - other->max.x;
	float d2y = self->min.y - other->max.y;

	if (d1x > 0.0f || d1y > 0.0f)
		return false;
	if (d2x > 0.0f || d2y > 0.0f)
		return false;

	return true;
}
