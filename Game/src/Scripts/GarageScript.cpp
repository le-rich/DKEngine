#include "Scripts/GarageScript.h"
#include "Scripts/OrbitScript.h"

#include <glm.hpp>

GarageScript::GarageScript(Entity* mEntity) : Script(mEntity)
{
}

GarageScript::~GarageScript() {}

void GarageScript::Update(float deltaTime)
{
    // hax since unbinding the same key within ('enter') chooseCallback callback crashes
    if (mParams.isCarSelected)
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

    mParams.selectCallback = [&](Input::ActionType action) {
        if (action == Input::PRESS) {
            this->SelectNextCar();
        }
        };
    
    input.RegisterKeyCallback(GLFW_KEY_SPACE, mParams.selectCallback);
}

void GarageScript::BindChooseKey() 
{
    Input& input = Input::GetInstance();

    mParams.chooseCallback = [&](Input::ActionType action) {
        if (action == Input::PRESS) {
            this->ChooseCar();
            if (mParams.chosenTarget) {
                leaveGarage();
            }
        }
        };

    input.RegisterKeyCallback(GLFW_KEY_ENTER, mParams.chooseCallback);
}

void GarageScript::UnbindAllControls()
{
    Input& input = Input::GetInstance();
    
    input.UnregisterKeyCallback(GLFW_KEY_ENTER, mParams.chooseCallback);
    input.UnregisterKeyCallback(GLFW_KEY_SPACE, mParams.selectCallback);
}

void GarageScript::SelectNextCar()
{
    int previousIndex = mParams.currSelectIndex;
    ++mParams.currSelectIndex;
    if (mParams.currSelectIndex > (mParams.cars.size() - 1))
    {
        mParams.currSelectIndex = 0;
    }
    
    // TODO: Unity-like behaviour with enabling/disabling entities instead of moving
    
    // move prev car out pedestal
    mParams.cars[previousIndex]->transform->setWorldPosition(glm::vec4(unSelectedPosX, 0.f, 0.f, 0.f));
    // move new curr car to pedestal
    mParams.cars[mParams.currSelectIndex]->transform->setWorldPosition(glm::vec4(0.f, 0.f, 0.f, 0.f));
    // set orbit target to new curr car
    mParams.orbitScript->setOrbitTarget(mParams.cars[mParams.currSelectIndex]->transform);
}

void GarageScript::ChooseCar()
{
    mParams.chosenTarget = mParams.cars[mParams.currSelectIndex]->transform;
    if (mParams.chosenTarget) { mParams.isCarSelected = true; }
}

void GarageScript::leaveGarage()
{
    // TODO: logic to transport currently selected car to the track
    mParams.chosenTarget->setWorldPosition(glm::vec4(100.0f, 0.f, 100.0f, 0.f));
}