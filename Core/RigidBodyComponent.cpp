#include "Entity.h"
#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(Entity* mEntity, float mass) 
	: Component(mEntity), mass(mass) {}

RigidBodyComponent::~RigidBodyComponent() {}


// getters and setter definitions

float RigidBodyComponent::getMass() const {
	return mass;
}

void RigidBodyComponent::setMass(float m) {
	mass = m;
}

const glm::vec3& RigidBodyComponent::getVelocity() const {
	return velocity;
}

void RigidBodyComponent::setVelocity(const glm::vec3& v) {
	velocity = v;
}

const glm::vec3& RigidBodyComponent::getForce() const {
	return force;
}

void RigidBodyComponent::applyForce(const glm::vec3& f) {
	force += f;
}

// other funcs

void RigidBodyComponent::update() {
	//TODO: actual update logic
}

// overloaded equality
bool RigidBodyComponent::operator==(const RigidBodyComponent& other) const {
	return Component::operator==(other) &&
		this->mass == other.mass &&
		this->velocity == other.velocity &&
		this->force == other.force;
}