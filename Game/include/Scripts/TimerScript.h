#pragma once

#include "Script.h"

#include "Components/TransformComponent.h"

struct TimerScriptParams : ScriptParams
{
	TransformComponent* m_TimerTarget = nullptr;

	glm::vec3 m_OriginalPosition = glm::vec3(0, 0, 0);

	float m_rawTime = 0.f;
	
	int m_Minutes = 0;
	int m_Seconds = 0;
	int m_Milliseconds = 0;
};

class TimerScript : public Script
{
public:
	TimerScript(Entity* mEntity);
	~TimerScript();

	void Update(float deltaTime) override;
	void SetParameters(ScriptParams* pScriptParameters) override;

	std::unique_ptr<Script> clone() const override
	{
		return std::make_unique<TimerScript>(*this);
	}

private:
	TimerScriptParams mParams{};
};