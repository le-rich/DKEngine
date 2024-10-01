#include "../include/pfgen.h"
using namespace AE86;
void PointMassForceRegistry::updateForces(real duration)
{
	Registry::iterator i = registrations.begin();
	for (; i != registrations.end(); i++)
	{
		i->fg->updateForce(i->pointMass, duration);
	}
}

void PointMassGravity::updateForce(PointMass* pointMass, real duration)
{
	// Check that we do not have infinite mass.
	// if (!particle->hasFiniteMass()) return;
	if (pointMass->getInverseMass() <= 0.0) return;
	// Apply the mass-scaled force to the particle.
	// particle->addForce(gravity * particle->getMass());
	pointMass->addForce(gravity * (1 / pointMass->getInverseMass()));
}
