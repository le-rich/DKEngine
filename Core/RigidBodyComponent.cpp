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


const glm::vec3 RigidBodyComponent::getForce() const {
	AE86::Vector3 forceAccum = rb.getForceAccum();

	return glm::vec3(forceAccum.x, forceAccum.y, forceAccum.z);
}

void RigidBodyComponent::applyForce(const glm::vec3& f) {
	rb.addForce(AE86::Vector3(f.x, f.y, f.z));
}

void RigidBodyComponent::applyForceAtPoint(const glm::vec3& f, const glm::vec3& p) {
	rb.addForceAtPoint(AE86::Vector3(f.x, f.y, f.z), AE86::Vector3(p.x, p.y, p.z));
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