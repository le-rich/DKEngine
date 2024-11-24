#include "Scripts/CarControllerScript.h"

#include <Components/RigidBodyComponent.h>
#include <Body.h>
#include <Input.h>

CarControllerScript::CarControllerScript(Entity* mEntity) : Script(mEntity)
{
	RigidBodyComponent* rigidBodyComponent = dynamic_cast<RigidBodyComponent*>(mEntity->getComponent(ComponentType::RigidBody));
	std::shared_ptr<AE86::RigidBody> carRigidBody = rigidBodyComponent->getRigidBody();
    carRigidBody->setAwake(true);
    carRigidBody->setInverseMass(0.95f);
    carRigidBody->setLinearDamping(0.50f);
    carRigidBody->setAngularDamping(0.50f);

    glm::quat initOrientation = entity->transform->getLocalOrientation();
    glm::vec3 initPosition = entity->transform->getLocalPosition();

    carRigidBody->setOrientation(AE86::Quaternion(initOrientation.w, initOrientation.x, initOrientation.y, initOrientation.z));
    carRigidBody->setPosition(AE86::Vector3(initPosition.x, initPosition.y, initPosition.z));
    carRigidBody->calculateDerivedData();

    SetUpInput();
}

CarControllerScript::~CarControllerScript()
{
}

void CarControllerScript::Update(float deltaTime)
{
}

void CarControllerScript::SetParameters(ScriptParams* pScriptParameters)
{
	mParams = *static_cast<CarControllerScriptParams*>(pScriptParameters);
}

void CarControllerScript::SetUpInput() {
    Input& input = Input::GetInstance();

    RigidBodyComponent* carRigidBodyComponent = dynamic_cast<RigidBodyComponent*>(
        entity->getComponent(ComponentType::RigidBody)
        );

    auto carRigidBody = carRigidBodyComponent->getRigidBody();

    input.RegisterKeyCallback(GLFW_KEY_W, [carRigidBody](Input::ActionType action) {
        if (action == Input::HOLD || action == Input::PRESS)
            carRigidBody->addForce(AE86::Vector3(0.0f, 0.0f, 5.0f));
    });

    input.RegisterKeyCallback(GLFW_KEY_S, [carRigidBody](Input::ActionType action) {
        if (action == Input::HOLD || action == Input::PRESS)
            carRigidBody->addForce(AE86::Vector3(0.0f, 0.0f, -5.0f));
    });
}