#pragma once

#include "Script.h"
#include "Components/TransformComponent.h"

class LapColliderScript : public Script
{
public:
	LapColliderScript(Entity* mEntity);
	~LapColliderScript();

	void Update(float deltaTime) override;

	void SetSelf(TransformComponent* transform) { m_Self = transform; }
	void SetOther(TransformComponent* transform) { m_Other = transform; }

	std::unique_ptr<Script> clone() const override {
		return std::make_unique<LapColliderScript>(*this);
	}
private:
	TransformComponent* m_Self = nullptr;
	TransformComponent* m_Other = nullptr;
};

