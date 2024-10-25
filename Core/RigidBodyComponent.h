#pragma once

#include "Component.h"
#include "../Physics/include/body.h"

#include <glm/glm.hpp>
#include <string>

class RigidBodyComponent : public Component {

protected:
	AE86::RigidBody rb;

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

