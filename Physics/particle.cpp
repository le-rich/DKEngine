#include <assert.h>
#include "particle.h"

using namespace AE86;

void Particle::setInverseMass(real invMass) {
	inverseMass = invMass;
}

void Particle::setMass(real mass) {
	inverseMass = (real)1.0 / mass;
}


void Particle::integrate(real duration) {

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
	velocity.addScaledVector(resultingAcc, duration);
}