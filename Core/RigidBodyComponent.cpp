#include "Entity.h"
#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(Entity* mEntity, float mass) 
	: Component(mEntity) {
	rb.setMass(mass);
}

RigidBodyComponent::~RigidBodyComponent() {}

// getters and setter definitions

float RigidBodyComponent::getMass() const {
	return rb.getMass();
}

void RigidBodyComponent::setMass(float m) {
	rb.setMass(m);
}

const glm::vec3 RigidBodyComponent::getVelocity() const {
	AE86::Vector3 velocity = rb.getVelocity();


	return glm::vec3(velocity.x, velocity.y, velocity.z);
}

void RigidBodyComponent::setVelocity(const glm::vec3& v) {
	rb.setVelocity(AE86::Vector3(v.x, v.y, v.z));
}

// other funcs

void RigidBodyComponent::update() {
	//TODO: actual update logic
}

// overloaded equality
bool RigidBodyComponent::operator==(const RigidBodyComponent& other) const {
	return Component::operator==(other) &&
		this->rb.getMass() == other.rb.getMass();
}