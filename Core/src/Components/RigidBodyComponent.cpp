#include "Entity.h"
#include "Components/RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(Entity* mEntity, std::shared_ptr<AE86::RigidBody> rigidBody, float mass)
	: Component(mEntity), rb(rigidBody) {
	rb->setMass(mass);
	this->componentType = ComponentType::RigidBody;
}

RigidBodyComponent::RigidBodyComponent(Entity* mEntity) : Component(mEntity) {
	std::shared_ptr<AE86::RigidBody> rb = std::shared_ptr<AE86::RigidBody>(new AE86::RigidBody());
	rb->setMass(1.0f);
	this->componentType = ComponentType::RigidBody;
}


RigidBodyComponent::~RigidBodyComponent() {}

// getters and setter definitions

std::shared_ptr<AE86::RigidBody> RigidBodyComponent::getRigidBody() {
	return rb;
}

float RigidBodyComponent::getMass() const {
	return rb->getMass();
}

void RigidBodyComponent::setMass(float m) {
	rb->setMass(m);
}

const glm::vec3 RigidBodyComponent::getVelocity() const {
	AE86::Vector3 velocity = rb->getVelocity();


	return glm::vec3(velocity.x, velocity.y, velocity.z);
}

void RigidBodyComponent::setVelocity(const glm::vec3& velocity) {
	rb->setVelocity(AE86::Vector3(velocity.x, velocity.y, velocity.z));
}


const glm::vec3 RigidBodyComponent::getForce() const {
	AE86::Vector3 forceAccum = rb->getForceAccum();

	return glm::vec3(forceAccum.x, forceAccum.y, forceAccum.z);
}

void RigidBodyComponent::applyForce(const glm::vec3& force) {
	rb->addForce(AE86::Vector3(force.x, force.y, force.z));
}

void RigidBodyComponent::applyForceAtPoint(const glm::vec3& force, const glm::vec3& position) {
	rb->addForceAtPoint(AE86::Vector3(force.x, force.y, force.z), 
		AE86::Vector3(position.x, position.y, position.z));
}


// other funcs

void RigidBodyComponent::update() {
	// TODO: fix potential race conditions by locking scene graph
	glm::vec3 worldPosition = entity->transform->getWorldPosition();
	glm::quat worldOrientation = entity->transform->getWorldOrientation();
	rb->setPosition(AE86::Vector3(worldPosition.x, worldPosition.y, worldPosition.z));
	auto ae86Quat = AE86::Quaternion(worldOrientation.w, worldOrientation.x, worldOrientation.y,
		worldOrientation.z);
	rb->setOrientation(ae86Quat);
}

// overloaded equality
bool RigidBodyComponent::operator==(const RigidBodyComponent& other) const {
	return Component::operator==(other) &&
		this->rb->getMass() == other.rb->getMass();
}

RigidBodyComponent::RigidBodyComponent(const RigidBodyComponent& other)
	: Component(other.entity),
	rb(std::make_shared<AE86::RigidBody>(*other.rb))
{}

Component* RigidBodyComponent::clone() const {
	return new RigidBodyComponent(*this);
}