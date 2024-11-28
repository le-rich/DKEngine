#include "Scripts/GarageScript.h"
#include "Scripts/OrbitScript.h"

#include <glm.hpp>
#include <Components/ScriptComponent.h>
#include <Components/RigidBodyComponent.h>
#include <Scripts/CarControllerScript.h>
#include <Scripts/FollowCamScript.h>
#include <Managers/EntityManager.h>
#include <Scripts/LapCheckpointScript.h>

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

    /* === BELOW code adds a car controller to the selected car. ==== */
    Entity* selectedCarEnt = mParams.cars[mParams.currSelectIndex];
    ScriptComponent* carScriptComponent = dynamic_cast<ScriptComponent*>(selectedCarEnt->getComponent(ComponentType::Script));
    if (!carScriptComponent) {
        carScriptComponent = new ScriptComponent(selectedCarEnt);
        selectedCarEnt->addComponent(*carScriptComponent);
    }


    RigidBodyComponent* rigidBodyComponent = new RigidBodyComponent(selectedCarEnt,
    std::shared_ptr<AE86::RigidBody>(new AE86::RigidBody()),
    1.0f);

    selectedCarEnt->addComponent(*rigidBodyComponent);

    // TODO: tie CarControllerParams to selected car via look-up, cleans up this method
    // and also required for different cars with different tuning.
    CarControllerScriptParams carParams;
    // these are all stats for an AE86 (with a little bit of modification):
    carParams.m_InverseMass =  1.0f / 1500.0f;
    carParams.m_Height = 1.335f;
    carParams.m_Width = 1.625f;
    carParams.m_Length = 4.185f;
    carParams.m_CGToFrontAxleDistance = 1.25f;
    carParams.m_CGToRearAxleDistance = 1.25f;
    carParams.m_CGHeight = 1.0f;
    carParams.m_WheelBase = 2.50f;
    carParams.m_WheelLength = 0.7f;
    carParams.m_WheelWidth = 0.3f;
    carParams.m_FrictionCoeff = 0.30f;
    carParams.m_FrontalArea = 2.2f;
    carParams.m_EngineForce = 8000.0f;
    carParams.m_BrakeForce = 12000.0f;
    carParams.m_DifferentialRatio = 3.42;
    carParams.m_WheelRadius = 0.33f;
    carParams.m_CorneringStiffness = -5.20f;
    carParams.m_TireGrip = 2.0f;
    carScriptComponent->CreateAndAddScript<CarControllerScript>(&carParams);
    carScriptComponent->GetScript<CarControllerScript>()->SetUpInput();
    /*=== DONE ADDING CAR CONTROLLER ===*/

    /*============ FIX CAMERA ============*/
    mParams.orbitScript->setOrbitTarget(nullptr); // set to no follow target, TODO: actually remove the script
    Entity* cameraEnt = EntityManager::getInstance().findFirstEntityByDisplayName("Main Camera");
    // starting position of camera to avoid initially slowly slerping to the track from the selection room
    // TODO: remove hard-coded magic numbers and actually derive the values from the camerascriptcomponent
    cameraEnt->transform->setLocalPosition(glm::vec3(100.0f, 1.65f, 97.0f));
    cameraEnt->transform->setLocalOrientation(glm::quat(0, 0, 1, 0));
    ScriptComponent* cameraScriptComponent = dynamic_cast<ScriptComponent*>(cameraEnt->getComponent(ComponentType::Script));
    FollowCamScriptParams followCamScriptParams;
    followCamScriptParams.m_FollowTarget = selectedCarEnt->transform;
    cameraScriptComponent->CreateAndAddScript<FollowCamScript>(&followCamScriptParams);

    /*============ SET UP CHECKPOINTS =========*/
    // need to set selected car's AABB.
    LapCheckpointScriptParams lapCheckpointScriptParams;
    lapCheckpointScriptParams.m_Other = selectedCarEnt->transform;

    Entity* gameManagerEnt = EntityManager::getInstance().findFirstEntityByDisplayName("GameManager");
    ScriptComponent* gameManagerScriptComponent = dynamic_cast<ScriptComponent*>(gameManagerEnt->getComponent(ComponentType::Script));
    LapManagerScript* lapManagerScript = gameManagerScriptComponent->GetScript<LapManagerScript>();
    lapCheckpointScriptParams.m_LapManager = lapManagerScript;
    lapManagerScript->SetCheckpointParams(&lapCheckpointScriptParams);


}