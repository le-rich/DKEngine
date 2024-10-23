#pragma once

#include "Component.h"

#include <glm/glm.hpp>
#include <string>

class RigidBodyComponent : public Component {

private:
	float mass;
	glm::vec3 velocity;
	glm::vec3 force;

public:
	// init to a default mass like unity?
	RigidBodyComponent(Entity* mEntity, float mass = 1.0f);
	~RigidBodyComponent();

	// getters and setters
	float getMass() const;
	void setMass(float m);

	const glm::vec3& getVelocity() const;
	void setVelocity(const glm::vec3& v);

	const glm::vec3& getForce() const;
	void applyForce(const glm::vec3& f);

	// other funcs
	void update();

	bool operator==(const RigidBodyComponent& other) const;

};

