#pragma once

#include "Component.h"
#include "../Physics/include/body.h"

#include <glm.hpp>
#include <string>

class RigidBodyComponent : public Component {

protected:
	std::shared_ptr<AE86::RigidBody> rb;

public:
	std::shared_ptr<AE86::RigidBody> getRigidBody();

	// init to a default mass like unity?
	RigidBodyComponent(Entity* mEntity, std::shared_ptr<AE86::RigidBody> rigidBody, float mass = 1.0f);
	
	// constructor that creates an empty rigidbody
	RigidBodyComponent(Entity* mEntity);
	~RigidBodyComponent();

	// getters and setters
	float getMass() const;
	void setMass(float m);

	const glm::vec3 getVelocity() const;
	void setVelocity(const glm::vec3& velocity);

	/** 
	 *  The current force applied to the object across the 3 cardinal
	 *  axes in Newtons. Once a physics world simulation occurs,
     *  this gets cleared and will be a (0,0,0) vector.
     */
	const glm::vec3 getForce() const;

	/** 
	 *  Apply a force, represented as a vector for each of the 3 cardinal 
	 *  axes in Newtons, applied on the centre of mass of the RigidBody.
	 *  This will result in no torque and therefore no angular acceleration.
	 */
	void applyForce(const glm::vec3& force);

	/**
	 *  Apply a force, represented as a vector for each of the 3 cardinal
	 *  axes in Newtons, applied on the world coordinates of the RigidBody.
	 *  This may result in torque and therefore angular acceleration.
	 */
	void applyForceAtPoint(const glm::vec3& force, const glm::vec3& position);


	// other funcs
	void update();

	bool operator==(const RigidBodyComponent& other) const;
	// copy constructor for clone
	RigidBodyComponent(const RigidBodyComponent& other);
	Component* clone() const override;

};

