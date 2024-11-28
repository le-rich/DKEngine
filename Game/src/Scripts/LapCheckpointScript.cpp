#include "Scripts/LapCheckpointScript.h"
#include "../../../Core/include/Managers/EntityManager.h"

LapCheckpointScript::LapCheckpointScript(Entity* mEntity) : Script(mEntity)
{
	mParams.m_Self = this->entity->transform;
}

LapCheckpointScript::~LapCheckpointScript()
{
}

void LapCheckpointScript::SetLapManager(LapManagerScript* lapManager)
{
	mParams.m_LapManager = lapManager;
}

void LapCheckpointScript::Update(float deltaTime)
{

	mParams.m_OtherBB->UpdateBounds(mParams.m_Other->getWorldPosition());

	if (!mParams.m_Registered && mParams.m_SelfBB->CheckCollision(*mParams.m_OtherBB))
	{
		// TODO: make this cleaner, maybe with a direct reference to the entity saved
		// instead.
		std::vector<Entity*> children = entity->getChildren();
		for (Entity* child : children) {
			Component* c = child->getComponent(ComponentType::Mesh);
			if (c)
				child->removeComponent(*c);
		}

		mParams.m_LapManager->OnCheckpointTriggered(mParams.m_Index);
	}
}

void LapCheckpointScript::addVisualIndicator(Entity* checkpointVisualEnt) {
}


void LapCheckpointScript::SetParameters(ScriptParams* pScriptParameters)
{
	mParams = *static_cast<LapCheckpointScriptParams*>(pScriptParameters);
	mParams.m_Self = this->entity->transform;
	mParams.m_SelfBB = new AE86::Aabb(glm::vec3(mParams.m_Self->getWorldPosition()), mParams.m_BBOffset);
	
	glm::vec3 otherPosition = glm::vec3(mParams.m_Other->getWorldPosition());
	mParams.m_OtherBB = new AE86::Aabb(otherPosition, mParams.m_BBOffset);
}
