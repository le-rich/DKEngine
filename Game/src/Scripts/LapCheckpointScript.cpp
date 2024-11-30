#include "Scripts/LapCheckpointScript.h"
#include "Managers/EntityManager.h"

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
		// instead, will let us remake the checkpoint on subsequent laps too.
		std::vector<Entity*> children = entity->getChildren();
		for (Entity* child : children) {
			Component* c = child->getComponent(ComponentType::Mesh);
			if (c) {
				// TODO: make this delete instead, currently there's a race condition if we do so now
				// rendering can crash trying to render a mesh that has been deleted.
				// its also an interesting question, deleting entities from game system,
				// we need to queue deletions so it happens at the beginning maybe.
				child->transform->setLocalPosition(glm::vec3(0.0f, -40.0f, 0.0f));
				// the race condition causing code in question.
				//child->removeComponent(*c);
			}
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
