#pragma once

#include "Script.h"


struct CarControllerScriptParams : ScriptParams
{
	double m_InverseMass;
	// all below units in metres
	float m_Width;
	float m_Height;
	float m_Length;
	float m_WheelBase;				
	float m_CGToFrontAxleDistance; 
	float m_CGToRearAxleDistance;  
	float m_WheelLength;
	float m_WheelWidth;
	float m_WheelRadius;
	float m_CGHeight;
	float m_FrictionCoeff;
	float m_FrontalArea;
	float m_EngineForce;
	float m_BrakeForce;
	float m_DifferentialRatio;
	float m_TireGrip;
	float m_CorneringStiffness;
	float m_WheelFL;
	float m_WheelFR;
	float m_WheelRR;
	float m_WheelRL;
};

class CarControllerScript : public Script
{
public:
	CarControllerScript(Entity* mEntity);
	~CarControllerScript();

	void Update(float deltaTime) override;
	void FixedUpdate(float deltaTime);
	void SetParameters(ScriptParams* pScriptParameters) override;
	std::unique_ptr<Script> clone() const override {
		return std::make_unique<CarControllerScript>(*this);
	}
	void SetUpInput();
private:
	CarControllerScriptParams mParams{};
	float lookUpTorqueCurve(float rpm);
	float steerAngle;
	float throttle;
	float brake;
	float rearAxleAngularVelocity;
	float gearRatio;
	float angularVelocity;
	glm::vec3 acceleration;
	glm::vec3 velocity;
	float eulerAngle;
	Entity* leftFrontTire;
	Entity* rightFrontTire;
	Entity* rearAxle;
};

