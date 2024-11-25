#include "Scripts/GarageScript.h"
#include "Scripts/OrbitScript.h"
#include "Input.h"

#include <glm.hpp>

GarageScript::GarageScript(Entity* mEntity) : Script(mEntity)
{
    bindOrbitKey();
    bindSelectionKey();
}

GarageScript::~GarageScript() {}

void GarageScript::Update(float deltaTime)
{
}

void GarageScript::SetParameters(ScriptParams* pScriptParameters)
{
    mParams = *static_cast<GarageScriptParams*>(pScriptParameters);  
}

void GarageScript::bindOrbitKey()
{
    Input& input = Input::GetInstance();
    
    input.RegisterKeyCallback(GLFW_KEY_SPACE, [&](Input::ActionType action) {
        if (action == Input::PRESS) {
            this->SelectNextCar();
        }
        });
}

void GarageScript::bindSelectionKey() 
{
    // TODO: bind enter to selected the target
    Input& input = Input::GetInstance();

    input.RegisterKeyCallback(GLFW_KEY_ENTER, [&](Input::ActionType action) {
        if (action == Input::PRESS) {
            this->ChooseCar();
            if (mParams.selectedTarget) { this->leaveGarage(); }
        }
        });
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