#define _USE_MATH_DEFINES
#include <math.h>
#include "Scripts/CarControllerScript.h"
#include <Components/RigidBodyComponent.h>
#include <Body.h>
#include <Input.h>

const static float AIR_DENSITY = 1.29f; // kg/m^3
const static float TRANSMISSION_EFFICIENCY = 0.7f;

CarControllerScript::CarControllerScript(Entity* mEntity) : Script(mEntity)
{
}

CarControllerScript::~CarControllerScript()
{
}

void CarControllerScript::Update(float deltaTime)
{
    std::cout << "STEERING ANGLE: " << steerAngle << "\n";
    std::cout << "THROTTLE: " << throttle << "\n";
    std::cout << "BRAKING:: " << brake << "\n";

    RigidBodyComponent* carRigidBodyComponent = dynamic_cast<RigidBodyComponent*>(
        entity->getComponent(ComponentType::RigidBody)
        );
    auto carRigidBody = carRigidBodyComponent->getRigidBody();

    // local velocity
    glm::vec3 velocity = glm::vec3(carRigidBody->getVelocity().x,
        carRigidBody->getVelocity().y,
        carRigidBody->getVelocity().z);

    glm::quat carRotation = glm::quat(carRigidBody->getOrientation().r,
        carRigidBody->getOrientation().i,
        carRigidBody->getOrientation().j,
        carRigidBody->getOrientation().k);

    velocity = carRotation * velocity;

    //velocity *= 2.6 * 60;

    double dragForce = 0.5 * mParams.m_FrictionCoeff * mParams.m_FrontalArea * AIR_DENSITY *
        glm::length(velocity) * glm::length(velocity);

    std::cout << "VELOCITY (X, Y, Z): (" << velocity.x << "," << velocity.y << "," << velocity.z << ")\n";

    double rollResistanceForce = 30.0f * dragForce; // 30.0f from paper
    double mass = carRigidBody->getMass();
    double weight = mass * 9.81;
    AE86::Vector3 acceleration = carRigidBody->getLastFrameAcceleration();

    double weightTransfer =
        (mParams.m_CGHeight / mParams.m_WheelBase) * mass * acceleration.magnitude();

    double frontAxleWeight =
        (mParams.m_CGToRearAxleDistance / mParams.m_WheelBase) * weight
        - weightTransfer;

    double rearAxleWeight =
        (mParams.m_CGToFrontAxleDistance / mParams.m_WheelBase) * weight
        + weightTransfer;

    rearAxleAngularVelocity = velocity.z / mParams.m_WheelRadius;

    double rpm = rearAxleAngularVelocity * gearRatio
        * mParams.m_DifferentialRatio * (60.0 / (2 * M_PI));

    if (rpm < 1000.0f)
        rpm = 1000.0f;

    double maxTorque = lookUpTorqueCurve(rpm);
    double engineTorque = throttle * maxTorque;
    double driveTorque = engineTorque
        * gearRatio * mParams.m_DifferentialRatio * TRANSMISSION_EFFICIENCY;
    double driveForce = driveTorque
        / mParams.m_WheelRadius;


    double tractionTorque = driveForce * mParams.m_WheelRadius;

    double totalTorque = driveTorque + (-2 * tractionTorque);

    // equation of a cylinder's inertia
    double rearAxleInertia = 75.0 * mParams.m_WheelRadius * mParams.m_WheelRadius;

    std::cout << "ANGULAR VEL: " << rearAxleAngularVelocity << "\n";
    //std::cout << "DRIVE FORCE: " << dragForce << "\n";

    carRigidBody->addForce(AE86::Vector3(0.0f, 0.0f, driveForce - dragForce - rollResistanceForce - brake * mParams.m_BrakeForce));

}

// static test torque curve, ideally an mParam look-up
float CarControllerScript::lookUpTorqueCurve(float rpm) {
    if (rpm >= 4400)
        return 400.0f;

    if (rpm >= 3000)
        return 250.0f;

    return 100.0f;
}

void CarControllerScript::SetParameters(ScriptParams* pScriptParameters)
{
	mParams = *static_cast<CarControllerScriptParams*>(pScriptParameters);
    RigidBodyComponent* rigidBodyComponent = dynamic_cast<RigidBodyComponent*>(entity->getComponent(ComponentType::RigidBody));
    std::shared_ptr<AE86::RigidBody> carRigidBody = rigidBodyComponent->getRigidBody();
    carRigidBody->setAwake(true);
    carRigidBody->setInverseMass(mParams.m_InverseMass);
    carRigidBody->setLinearDamping(1.0f);
    carRigidBody->setAngularDamping(1.0f);

    glm::quat initOrientation = entity->transform->getLocalOrientation();
    glm::vec3 initPosition = entity->transform->getLocalPosition();

    carRigidBody->setOrientation(AE86::Quaternion(initOrientation.w, initOrientation.x, initOrientation.y, initOrientation.z));
    carRigidBody->setPosition(AE86::Vector3(initPosition.x, initPosition.y, initPosition.z));
    carRigidBody->calculateDerivedData();

    /** Rectangular block inertia tensor calculated below **/
    auto massScale = (1.0f / 12.0f) * (1 / mParams.m_InverseMass);
    auto height = mParams.m_Height;
    auto width = mParams.m_Width;
    auto length = mParams.m_Length;
    AE86::real zero = 0;
    carRigidBody->setInertiaTensor(AE86::Matrix3(
         (AE86::real) massScale * (height * height + length * length), 0.0f, 0.0f,
         0.0f, (AE86::real) massScale * (width * width * length * length), 0.0f,
         0.0f, 0.0f, (AE86::real)massScale * (width * width * height * height)
    ));
}

void CarControllerScript::SetUpInput() {
    gearRatio = 2.66f;
    Input& input = Input::GetInstance();

    RigidBodyComponent* carRigidBodyComponent = dynamic_cast<RigidBodyComponent*>(
        entity->getComponent(ComponentType::RigidBody)
        );


    auto carRigidBody = carRigidBodyComponent->getRigidBody();

    input.RegisterKeyCallback(GLFW_KEY_W, [carRigidBody, this](Input::ActionType action) {
        if (action == Input::HOLD || action == Input::PRESS) {
            throttle += throttle < 100.0f ? 10.0f : 0.0;
            //carRigidBody->addForce(AE86::Vector3(0.0f, 0.0f, 5000.0f));
        }

        if (action == Input::RELEASE) {
            throttle = 0.0f;
        }
    });

    input.RegisterKeyCallback(GLFW_KEY_S, [carRigidBody, this](Input::ActionType action) {
        if (action == Input::HOLD || action == Input::PRESS) {
            throttle -= throttle >= 10 ? 10.0f : 0.0;
           // carRigidBody->addForce(AE86::Vector3(0.0f, 0.0f, -5000.0f));
        }
    });

    input.RegisterKeyCallback(GLFW_KEY_A, [&](Input::ActionType action) {
        if (action == Input::HOLD || action == Input::PRESS)
            steerAngle -= steerAngle > (-M_PI / 4.0f) ? M_PI / 32.0f : 0;
     });

    input.RegisterKeyCallback(GLFW_KEY_D, [&](Input::ActionType action) {
        if (action == Input::HOLD || action == Input::PRESS) {
            steerAngle += steerAngle < (M_PI / 4.0f) ? M_PI / 32.0f : 0;
        }
      });

    input.RegisterKeyCallback(GLFW_KEY_SPACE, [&](Input::ActionType action) {
        if (action == Input::HOLD || action == Input::PRESS) {
            brake = 100.0f;
            throttle = 0.0f;
        }
  
        if (action == Input::RELEASE)
            brake = 0;
    });
}