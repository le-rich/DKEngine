#include "Scripts/GarageScript.h"
#include "Scripts/OrbitScript.h"
#include "Input.h"

GarageScript::GarageScript(Entity* mEntity) : Script(mEntity)
{
    bindOrbitKey();
    bindSelectionKey();
}

GarageScript::~GarageScript() {}

void GarageScript::Update(float deltaTime)
{
    // TODO: loop to check if a car has been selected yet
}

void GarageScript::SetParameters(ScriptParams* pScriptParameters)
{
    mParams = *static_cast<GarageScriptParams*>(pScriptParameters);  
}

void GarageScript::bindOrbitKey()
{
    Input& input = Input::GetInstance();
    OrbitScript* morbitScript = mParams.orbitScript;
    
    input.RegisterKeyCallback(GLFW_KEY_SPACE, [morbitScript](Input::ActionType action) {
        if (action == Input::PRESS) {
            morbitScript->swapOrbitTargets();
        }
        });
    
}

void GarageScript::bindSelectionKey() 
{
    // TODO: bind enter to selected the target
}

void GarageScript::leaveGarage()
{
    // TODO: logic to transport currently selected car to the track
}