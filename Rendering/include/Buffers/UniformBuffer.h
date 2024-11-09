#pragma once

#include <glad/glad.h>
#include <glm.hpp>


class UniformBuffer
{
public:
	UniformBuffer();
	~UniformBuffer();

	void Bind() const;
	void Unbind() const;

	void updateMatrices(const glm::mat4& model, const glm::mat4& view,
		const glm::mat4& projection,
		const glm::vec3& cameraPos);

	void SetCameraMatrices(const glm::mat4& view,
		const glm::mat4& projection,
		const glm::vec3& cameraPos);

	template<typename T>
	void SetSubData(const T& p_data, size_t&& p_offset)
	{
		Bind();
		glBindBuffer(GL_UNIFORM_BUFFER, mBufferID);
		glBufferSubData(GL_UNIFORM_BUFFER, std::forward<size_t>(p_offset), sizeof(T), std::addressof(p_data));
		Unbind();
	}

	template<typename T>
	void SetSubData(const T& p_data, size_t& p_offsetInOut)
	{
		//Bind();
		glBindBuffer(GL_UNIFORM_BUFFER, mBufferID);
		const size_t dataSize = sizeof(T);
		glBufferSubData(GL_UNIFORM_BUFFER, p_offsetInOut, dataSize, std::addressof(p_data));
		p_offsetInOut += dataSize;
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		//Unbind();
	}

private:
	GLuint mBufferID;
	const GLuint BINDING_POINT = 0;

};
