#define _USE_MATH_DEFINES
#include <math.h>
#include "Scripts/CarControllerScript.h"
#include <Components/RigidBodyComponent.h>
#include <Body.h>
#include <Input.h>
#include <Components/AudioComponent.h>

const static float AIR_DENSITY = 1.29f; // kg/m^3
const static float TRANSMISSION_EFFICIENCY = 0.7f;
const static float MAX_STEERING = (2 * M_PI / 45.0f); // TODO: tweak steering max

std::chrono::high_resolution_clock::time_point lastUpdateTime;

CarControllerScript::CarControllerScript(Entity* mEntity) : Script(mEntity)
{
    lastUpdateTime = std::chrono::high_resolution_clock::now();
    wheelFL = entity->findFirstChildByDisplayName("WheelFL");
    wheelFR = entity->findFirstChildByDisplayName("WheelFR");
    wheelRL = entity->findFirstChildByDisplayName("WheelRL");
    wheelRR = entity->findFirstChildByDisplayName("WheelRR");

    AudioComponent* carAudioComponent = dynamic_cast<AudioComponent*>(
        entity->getComponent(ComponentType::Audio)
        );
    FMOD::Channel* channel = carAudioComponent->GetChannel();
    if (channel) {
        channel->setFrequency(currentFrequency);
    }
}

CarControllerScript::~CarControllerScript()
{
}

// TODO: a better way to do fixed updates.
double timeAccumulator = 0.0;
const double FIXED_TIME_STEP = 0.008; // 8 milliseconds in seconds

AE86::Vector3 calculateTireForce(AE86::RigidBody* carRigidBody, Entity* tire, float deltaTime) {
    glm::vec3 tireWorldPos = tire->transform->getWorldPosition();
    glm::vec3 steeringDir = tire->transform->getRight();
    AE86::Vector3 tireVel = carRigidBody->getVelocityAtWorldPoint(
        AE86::Vector3(tireWorldPos.x, tireWorldPos.y, tireWorldPos.z)
    );

    float steeringVel = glm::dot(steeringDir, glm::vec3(tireVel.x, tireVel.y, tireVel.z));

    float desiredVelChange = -steeringVel * 1.0f;

    float desiredAcceleration = desiredVelChange / deltaTime;

    AE86::Vector3 finalLatForce = AE86::Vector3(steeringDir.x, steeringDir.y, steeringDir.z)
        * 75.0f * desiredAcceleration;

    return finalLatForce;
}


void CarControllerScript::Update(float deltaTime) {
    // Record the current time
    auto currentTime = std::chrono::high_resolution_clock::now();
    double elapsedTime = std::chrono::duration<double>(currentTime - lastUpdateTime).count();
    lastUpdateTime = currentTime;

    timeAccumulator += elapsedTime;

    while (timeAccumulator >= FIXED_TIME_STEP) {
        FixedUpdate(FIXED_TIME_STEP);
        timeAccumulator -= FIXED_TIME_STEP;
    }
}

// TODO: tweak and tune all of this, remove magic constants
void CarControllerScript::FixedUpdate(float deltaTime)
{
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

    double dragForce = 0.5 * mParams.m_FrictionCoeff * mParams.m_FrontalArea * AIR_DENSITY *
        glm::length(velocity) * glm::length(velocity);

    double rollResistanceForce = 30.0f * dragForce; // 30.0f from paper
    double mass = carRigidBody->getMass();
    double weight = mass * 9.81;
    AE86::Vector3 acceleration = carRigidBody->getLastFrameAcceleration();

    // TODO: incorporate weight transfer into pitch/nosedive of car as it brakes/accels
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


    // TODO: fix brakeforce bug.
    double brakeForce = brake * mParams.m_BrakeForce * ((velocity.z > 0) - (velocity.z < 0));

    driveForce += brakeForce;

    double tractionTorque = driveForce * mParams.m_WheelRadius;

    double totalTorque = driveTorque + (-2 * tractionTorque);

    // equation of a cylinder's inertia
    double rearAxleInertia = 75.0 * mParams.m_WheelRadius * mParams.m_WheelRadius;

    wheelFL->transform->setLocalOrientation(glm::angleAxis(-steerAngle, glm::vec3(0, 1, 0)));
    wheelFR->transform->setLocalOrientation(glm::angleAxis(steerAngle, glm::vec3(0, 1, 0)));
    
    glm::vec3 finalLongForce = carRotation * glm::vec3(0.0f, 0.0f, driveForce - dragForce - rollResistanceForce);


    // TODO: verify that all of this is correct, clean up to remove redundant code.
    AE86::Vector3 frontRightTirePosition =
        AE86::Vector3(-(mParams.m_Width / 2.0f), 0.0f, mParams.m_CGToFrontAxleDistance);
    AE86::Vector3 frontRightTireLatForce = calculateTireForce(carRigidBody.get(), wheelFR, deltaTime);

    AE86::Vector3 frontLeftTirePosition =
        AE86::Vector3((mParams.m_Width / 2.0f), 0.0f, -mParams.m_CGToFrontAxleDistance);
    AE86::Vector3 frontLeftTireLatForce = calculateTireForce(carRigidBody.get(), wheelFL, deltaTime);

    AE86::Vector3 backLeftTirePosition =
        AE86::Vector3(-(mParams.m_Width / 2.0f), 0.0f, -mParams.m_CGToRearAxleDistance);
    AE86::Vector3 backLeftTireLatForce = calculateTireForce(carRigidBody.get(), wheelRL, deltaTime);

    AE86::Vector3 backRightTirePosition =
        AE86::Vector3((mParams.m_Width / 2.0f), 0.0f, -mParams.m_CGToRearAxleDistance);
    AE86::Vector3 backRightTireLatForce = calculateTireForce(carRigidBody.get(), wheelRR, deltaTime);

    // TODO: VERIFY THE LATERAL FORCES
    carRigidBody->addForce(AE86::Vector3(finalLongForce.x, finalLongForce.y, finalLongForce.z));
    carRigidBody->addForceAtBodyPoint(frontLeftTireLatForce, frontLeftTirePosition);
    carRigidBody->addForceAtBodyPoint(frontRightTireLatForce, frontRightTirePosition);
    carRigidBody->addForceAtBodyPoint(backRightTireLatForce, backRightTirePosition);
    carRigidBody->addForceAtBodyPoint(backLeftTireLatForce, backLeftTirePosition);

    AudioComponent* carAudioComponent = dynamic_cast<AudioComponent*>(
        entity->getComponent(ComponentType::Audio)
        );

    FMOD::Channel* channel = carAudioComponent->GetChannel();
    if (channel) {
        channel->setFrequency(currentFrequency);
    }

    if (!isAccelerating) {
        currentFrequency += currentFrequency > minFrequency ? -500 : 0;
    }
}


// static test torque curve, ideally an mParam look-up
// TODO: make this pull from mParams for custom torque curves
// per car.
float CarControllerScript::lookUpTorqueCurve(float rpm) {
    if (rpm >= 4400)
        return 50.0f;

    if (rpm >= 3000)
        return 30.0f;

    return 15.0f;
}


void CarControllerScript::SetParameters(ScriptParams* pScriptParameters)
{
	mParams = *static_cast<CarControllerScriptParams*>(pScriptParameters);
    RigidBodyComponent* rigidBodyComponent = dynamic_cast<RigidBodyComponent*>(entity->getComponent(ComponentType::RigidBody));
    std::shared_ptr<AE86::RigidBody> carRigidBody = rigidBodyComponent->getRigidBody();
    carRigidBody->setAwake(true);
    carRigidBody->setInverseMass(mParams.m_InverseMass);
    carRigidBody->setLinearDamping(0.5f);
    carRigidBody->setAngularDamping(0.5f);

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

    input.RegisterKeyCallback(GLFW_KEY_W, [&](Input::ActionType action) {
        if (action == Input::HOLD || action == Input::PRESS) {
            throttle += throttle < 100.0f ? 10.0f : 0.0;
            if (currentFrequency < 3 * maxFrequency / 4) {
                currentFrequency += currentFrequency < maxFrequency ? 1000 : -1000;
            }
            else if (currentFrequency < 9 * maxFrequency / 10) {
                currentFrequency += currentFrequency < maxFrequency ? 250 : -250;
            }
            else {
                currentFrequency += currentFrequency < maxFrequency ? 100 : -100;
            }
            isAccelerating = true;
        }

        if (action == Input::RELEASE) {
            throttle = 0.0f; // TODO: have the throttle slowly reset to zero instead.
            isAccelerating = false;
        }
    });

    input.RegisterKeyCallback(GLFW_KEY_S, [&](Input::ActionType action) {
        if (action == Input::HOLD || action == Input::PRESS) {
            throttle -= throttle >= 10 ? 10.0f : 0.0;
            if (currentFrequency < 5 * maxFrequency / 8) {
                currentFrequency += currentFrequency < maxFrequency ? 1000 : -5000;
            }
            else {
                currentFrequency += currentFrequency < 2 * maxFrequency / 3 ? 100 : -100;
            }
            isAccelerating = true;
        }
        if (action == Input::RELEASE) {
            isAccelerating = false;
        }
    });

    input.RegisterKeyCallback(GLFW_KEY_D, [&](Input::ActionType action) {
        if (action == Input::HOLD || action == Input::PRESS)
            steerAngle -= steerAngle > -MAX_STEERING ? M_PI / 40.0f : 0;

        if (action == Input::RELEASE)
            steerAngle = 0.0f; // TODO: have the steering angle slowly reset to zero instead.
     });

    input.RegisterKeyCallback(GLFW_KEY_A, [&](Input::ActionType action) {
        if (action == Input::HOLD || action == Input::PRESS)
            steerAngle += steerAngle < MAX_STEERING ? M_PI / 40.0f : 0;
        
        if (action == Input::RELEASE)
            steerAngle = 0.0f; // TODO: have the steering angle slowly reset to zero instead.
         
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