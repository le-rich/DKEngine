#include "Scripts/TimerScript.h"

#include "Components/TransformComponent.h"

TimerScript::TimerScript(Entity* mEntity) : Script(mEntity)
{

}

TimerScript::~TimerScript() {}

void TimerScript::Update(float deltaTime)
{
	mParams.m_rawTime += deltaTime;

	if (mParams.m_TimerTarget->getWorldPosition().x > mParams.m_OriginalPosition.x)
	{
		mParams.m_Minutes = (int)mParams.m_rawTime / 60;
		mParams.m_Seconds = (int)mParams.m_rawTime % 60;
		mParams.m_Milliseconds = (int)((mParams.m_rawTime - (int)mParams.m_rawTime % 60) * 1000);
		std::cout << "Time elapsed: " << mParams.m_Minutes << ":" << mParams.m_Seconds
			<< "." << mParams.m_Milliseconds << std::endl;
	}
}

void TimerScript::SetParameters(ScriptParams* pScriptParameters)
{
	mParams = *static_cast<TimerScriptParams*>(pScriptParameters);
}