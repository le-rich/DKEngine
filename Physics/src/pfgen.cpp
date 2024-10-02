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

void PointMassForceRegistry::add(PointMass* pointMass, PointMassForceGenerator* fg)
{
	PointMassForceRegistry::PointMassForceRegistration registration;
	registration.pointMass = pointMass;
	registration.fg = fg;
	registrations.push_back(registration);
}

void PointMassGravity::updateForce(PointMass* pointMass, real duration)
{
	// Check that we do not have infinite mass.
	// if (!pointMass->hasFiniteMass()) return;
	if (pointMass->getInverseMass() <= 0.0) return;
	// Apply the mass-scaled force to the pointMass.
	pointMass->addForce(gravity * pointMass->getMass());
}

void PointMassDrag::updateForce(PointMass* pointMass, real duration)
{
	Vector3 force;
	// Points force to the value of the velocity of
	// the pointMass
	pointMass->getVelocity(&force);
	// Calculate the total drag coefficient.
	real dragCoeff = force.magnitude();
	dragCoeff = k1 * dragCoeff + k2 * real_pow(dragCoeff, 2);
	// Calculate the final force and apply it.
	force.normalize();
	force *= -dragCoeff;
	pointMass->addForce(force);
}

