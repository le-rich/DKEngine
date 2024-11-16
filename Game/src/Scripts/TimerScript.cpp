#include "Scripts/TimerScript.h"

#include "Components/TransformComponent.h"

#include <iomanip>

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
		mParams.m_Milliseconds = (int)((mParams.m_rawTime - (int) mParams.m_rawTime) * 1000);
		std::cout << "Time elapsed: " << std::setfill('0') << std::setw(2) << mParams.m_Minutes << ":" <<
			std::setfill('0') << std::setw(2) << mParams.m_Seconds << "." << mParams.m_Milliseconds << std::endl;
		std::cout << "Split " << mParams.m_Split << std::endl;
	}

	if ((int)mParams.m_TimerTarget->getWorldPosition().x % 5 == 0 &&
		(int)mParams.m_TimerTarget->getWorldPosition().x != 0 && !mParams.updatedSplit)
	{
		mParams.m_Split += 1;
		std::cout << "Split " << mParams.m_Split << ": " << std::setfill('0') << std::setw(2) << mParams.m_Minutes
			<< ":" << std::setfill('0') << std::setw(2) << mParams.m_Seconds<< "." << mParams.m_Milliseconds << std::endl;
		mParams.updatedSplit = true;
	}

	if ((int)mParams.m_TimerTarget->getWorldPosition().x % 5 != 0)
	{
		mParams.updatedSplit = false;
	}
}

void TimerScript::SetParameters(ScriptParams* pScriptParameters)
{
	mParams = *static_cast<TimerScriptParams*>(pScriptParameters);
}