#include "Scripts/GarageScript.h"
#include "Scripts/OrbitScript.h"

#include <glm.hpp>

GarageScript::GarageScript(Entity* mEntity) : Script(mEntity)
{
}

GarageScript::~GarageScript() {}

void GarageScript::Update(float deltaTime)
{
    // hax since unbinding the same key within ('enter') mChooseCallback callback crashes
    if (mParams.mIsCarSelected)
    {
        UnbindAllControls();
    }
}

void GarageScript::SetParameters(ScriptParams* pScriptParameters)
{
    mParams = *static_cast<GarageScriptParams*>(pScriptParameters);  
}

void GarageScript::BindSelectKey()
{
    Input& input = Input::GetInstance();

    mParams.mSelectCallback = [&](Input::ActionType action) {
        if (action == Input::PRESS) {
            this->SelectNextCar();
        }
        };
    
    input.RegisterKeyCallback(GLFW_KEY_SPACE, mParams.mSelectCallback);
}

void GarageScript::BindChooseKey() 
{
    Input& input = Input::GetInstance();

    mParams.mChooseCallback = [&](Input::ActionType action) {
        if (action == Input::PRESS) {
            this->ChooseCar();
            if (mParams.mChosenTarget) {
                leaveGarage();
            }
        }
        };

    input.RegisterKeyCallback(GLFW_KEY_ENTER, mParams.mChooseCallback);
}

void GarageScript::UnbindAllControls()
{
    Input& input = Input::GetInstance();
    
    input.UnregisterKeyCallback(GLFW_KEY_ENTER, mParams.mChooseCallback);
    input.UnregisterKeyCallback(GLFW_KEY_SPACE, mParams.mSelectCallback);
}

void GarageScript::SelectNextCar()
{
    ++mParams.currSelectIndex;
    if (mParams.currSelectIndex > (mParams.mCars.size() - 1))
    {
        mParams.currSelectIndex = 0;
    }
    mParams.mOrbitScript->setOrbitTarget(mParams.mCars[mParams.currSelectIndex]->transform);
}

void GarageScript::ChooseCar()
{
    mParams.mChosenTarget = mParams.mCars[mParams.currSelectIndex]->transform;
    if (mParams.mChosenTarget) { mParams.mIsCarSelected = true; }
}

void GarageScript::leaveGarage()
{
    // TODO: logic to transport currently selected car to the track
    mParams.mChosenTarget->setWorldPosition(glm::vec4(100.0f, 0.f, 100.0f, 0.f));
}