#include "Scripts/TimerScript.h"

#include "Components/TransformComponent.h"

TimerScript::TimerScript(Entity* mEntity) : Script(mEntity)
{

}

TimerScript::~TimerScript() {}

void TimerScript::Update(float deltaTime)
{
	if (mParams.m_TimerTarget->getWorldPosition().x > mParams.m_OriginalPosition.x)
	{
		std::cout << "Time elasped: " << deltaTime << std::endl;
		/*mParams.m_Minutes = (int)deltaTime / 60;
		mParams.m_Seconds = (int)deltaTime % 60;
		std::cout << "Time elapsed: " << mParams.m_Minutes << ":" << mParams.m_Seconds << std::endl;*/
	}
}

void TimerScript::SetParameters(ScriptParams* pScriptParameters)
{
	mParams = *static_cast<TimerScriptParams*>(pScriptParameters);
}