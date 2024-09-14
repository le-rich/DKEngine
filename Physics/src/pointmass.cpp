#include <assert.h>
#include "../include/pointmass.h"

using namespace AE86;

void PointMass::setInverseMass(real invMass) {
	inverseMass = invMass;
}

void PointMass::setMass(real mass) {
	inverseMass = (real)1.0 / mass;
}

real PointMass::getKineticEnergy() {
	return 0.5 * (1.0f / inverseMass) * velocity.magnitude();
}


void PointMass::integrate(real duration) {

	// don't integrate if the particle has infinite mass
	if (inverseMass <= 0.0f) return;

	assert(duration > 0.0);

	// update linear position
	position.addScaledVector(velocity, duration);

	// work out the acceleration from the force.
	// (more will be added here when we generate forces)
	Vector3 resultingAcc = acceleration;

	// update linear velocity from acceleration
	velocity.addScaledVector(resultingAcc, duration);

	// impose drag/damping
	velocity *= real_pow(damping, duration);
}