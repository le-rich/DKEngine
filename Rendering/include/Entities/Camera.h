#pragma once
#include "../../Core/include/Entity.h"

#include <../glm/glm.hpp>
#include <../glm/gtc/matrix_transform.hpp>

class Camera : Entity
{
public:


private:
	glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 m_cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_cameraDirection = glm::normalize(m_cameraPos - m_cameraTarget);
	glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_cameraRight = glm::normalize(glm::cross(m_up, m_cameraDirection));
	glm::vec3 cameraUp = glm::cross(m_cameraDirection, m_cameraRight);

	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
};

