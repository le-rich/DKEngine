#pragma once
#include "Script.h"
#include "LapManagerScript.h"
#include "Aabb.h"

#include <functional>

struct LapCheckpointScriptParams : ScriptParams
{
	TransformComponent* m_Self = nullptr;
	TransformComponent* m_Other = nullptr;
	AE86::Aabb* m_SelfBB = nullptr;
	AE86::Aabb* m_OtherBB = nullptr;
	float m_BBOffset = 0.25f;

	LapManagerScript* m_LapManager = nullptr;
	int m_Index = 0;
	bool m_Registered = false;
	Entity* m_SourceModel = nullptr;
};

class LapCheckpointScript : public Script
{
public:
	LapCheckpointScript(Entity* mEntity);
	~LapCheckpointScript();

	void SetLapManager(LapManagerScript* lapManager);
	bool GetCheckpointRegistered() const { return mParams.m_Registered; }
	void SetCheckpointRegistered(bool isRegistered) { mParams.m_Registered = isRegistered; }

	void addVisualIndicator(Entity* checkpointVisualEnt);

	void Update(float deltaTime) override;
	void SetParameters(ScriptParams* pScriptParameters) override;
	std::unique_ptr<Script> clone() const override {
		return std::make_unique<LapCheckpointScript>(*this);
	}
private:
	LapCheckpointScriptParams mParams{};
};

