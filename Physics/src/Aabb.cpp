#include "../include/Aabb.h"

AE86::Aabb::Aabb(glm::vec3 min, glm::vec3 max) : m_Min(min), m_Max(max), m_BBOffset(0.25f)
{
}

AE86::Aabb::~Aabb()
{
}

void AE86::Aabb::UpdateBounds(glm::vec3 newMin, glm::vec3 newMax)
{
	m_Min = newMin;
	m_Max = newMax;
}

void AE86::Aabb::UpdateBounds(glm::vec3 position)
{
	m_Min.x = position.x - m_BBOffset;
	m_Min.y = position.y - m_BBOffset;
	m_Min.z = position.z - m_BBOffset;
	m_Max.x = position.x + m_BBOffset;
	m_Max.y = position.y + m_BBOffset;
	m_Max.z = position.z + m_BBOffset;
}

bool AE86::Aabb::CheckCollision(Aabb other)
{
	float d1x = other.m_Min.x - this->m_Max.x;
	float d1y = other.m_Min.y - this->m_Max.y;
	float d1z = other.m_Min.z - this->m_Max.z;
	float d2x = this->m_Min.x - other.m_Max.x;
	float d2y = this->m_Min.y - other.m_Max.y;
	float d2z = this->m_Min.z - other.m_Max.z;

	if (d1x > 0.0f || d1y > 0.0f || d1z > 0.0f)
		return false;
	if (d2x > 0.0f || d2y > 0.0f || d2z > 0.0f)
		return false;

	return true;
}
