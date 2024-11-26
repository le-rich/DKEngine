#include "Scripts/GarageScript.h"
#include "Scripts/OrbitScript.h"

#include <glm.hpp>

GarageScript::GarageScript(Entity* mEntity) : Script(mEntity)
{
}

GarageScript::~GarageScript() {}

void GarageScript::Update(float deltaTime)
{
    // hax since unbinding the same key within ('enter') selectionKeyCallback callback crashes
    if (mParams.carHasBeenSelected)
    {
        unbindAllControls();
    }
}

void GarageScript::SetParameters(ScriptParams* pScriptParameters)
{
    mParams = *static_cast<GarageScriptParams*>(pScriptParameters);  
}

void GarageScript::bindOrbitKey()
{
    Input& input = Input::GetInstance();

    mParams.orbitKeyCallback = [&](Input::ActionType action) {
        if (action == Input::PRESS) {
            this->SelectNextCar();
        }
        };
    
    input.RegisterKeyCallback(GLFW_KEY_SPACE, mParams.orbitKeyCallback);
}

void GarageScript::bindSelectionKey() 
{
    Input& input = Input::GetInstance();

    mParams.selectionKeyCallback = [&](Input::ActionType action) {
        if (action == Input::PRESS) {
            this->ChooseCar();
            if (mParams.selectedTarget) {
                leaveGarage();
            }
        }
        };

    input.RegisterKeyCallback(GLFW_KEY_ENTER, mParams.selectionKeyCallback);
}

void GarageScript::unbindAllControls()
{
    Input& input = Input::GetInstance();
    
    input.UnregisterKeyCallback(GLFW_KEY_ENTER, mParams.selectionKeyCallback);
    input.UnregisterKeyCallback(GLFW_KEY_SPACE, mParams.orbitKeyCallback);
}

void GarageScript::SelectNextCar()
{
    ++mParams.currSelectIndex;
    if (mParams.currSelectIndex > (mParams.cars.size() - 1))
    {
        mParams.currSelectIndex = 0;
    }
    mParams.orbitScript->setOrbitTarget(mParams.cars[mParams.currSelectIndex]->transform);
}

void GarageScript::ChooseCar()
{
    mParams.selectedTarget = mParams.cars[mParams.currSelectIndex]->transform;
    if (mParams.selectedTarget) { mParams.carHasBeenSelected = true; }
}

void GarageScript::leaveGarage()
{
    // TODO: logic to transport currently selected car to the track
    mParams.selectedTarget->setWorldPosition(glm::vec4(100.0f, 0.f, 100.0f, 0.f));
}