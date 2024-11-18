#pragma once
#ifndef BODY_HEADER

#define BODY_HEADER

#include <glm.hpp>

namespace AE86 {
	class Aabb
	{
	public:
		Aabb(const glm::vec3 min, const glm::vec3 max);
		~Aabb();

		glm::vec3 GetMinBound() { return m_Min; }
		void SetMinBound(const glm::vec3 newMin) { m_Min = newMin; }
		glm::vec3 GetMaxBound() const { return m_Max; }
		void SetMaxBound(const glm::vec3 newMax) { m_Max = newMax; }

		void UpdateBounds(const glm::vec3 newMin, const glm::vec3 newMax);
		void UpdateBounds(const glm::vec3 position);
		bool CheckCollision(Aabb other);
	protected:
		glm::vec3 m_Min;
		glm::vec3 m_Max;
		float m_BBOffset = 0.25f;

	};
}

#endif