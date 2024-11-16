#pragma once

#include "Script.h"

class TimerScript : public Script
{
public:
	TimerScript(Entity* mEntity);
	~TimerScript();

	void Update(float deltaTime) override;
};