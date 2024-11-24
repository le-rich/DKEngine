#pragma once

#include "Script.h"


struct CarControllerScriptParams : ScriptParams
{
	float m_InverseMass;
	float m_Width;
	float m_Height;
	float m_Length;
};

class CarControllerScript : public Script
{
public:
	CarControllerScript(Entity* mEntity);
	~CarControllerScript();

	void Update(float deltaTime) override;
	void SetParameters(ScriptParams* pScriptParameters) override;
	std::unique_ptr<Script> clone() const override {
		return std::make_unique<CarControllerScript>(*this);
	}
private:
	CarControllerScriptParams mParams{};
	void SetUpInput();
};

