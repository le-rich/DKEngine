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

PointMassGravity::PointMassGravity(const Vector3& gravity) : gravity(gravity)
{
}

void PointMassGravity::updateForce(PointMass* pointMass, real duration)
{
	// Check that we do not have infinite mass.
	// if (!pointMass->hasFiniteMass()) return;
	if (pointMass->getInverseMass() <= 0.0) return;
	// Apply the mass-scaled force to the pointMass.
	pointMass->addForce(gravity * pointMass->getMass());
}

PointMassDrag::PointMassDrag(real k1, real k2) : k1(k1), k2(k2)
{
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

PointMassSpring::PointMassSpring(PointMass* other, real sc, real rl) : other(other), springConstant(sc), restLength(rl)
{
}

void PointMassSpring::updateForce(PointMass* pointMass, real duration)
{
	Vector3 force;
	pointMass->getPosition(&force);
	force -= other->getPosition();

	// Force magnitude calculation
	real magnitude = force.magnitude();
	magnitude = real_abs(magnitude - restLength);
	magnitude *= springConstant;

	// Calculate final force then apply
	force.normalize();
	force *= -magnitude;
	pointMass->addForce(force);
}

PointMassAnchoredSpring::PointMassAnchoredSpring(Vector3* anchor,
	real sc, real rl) : anchor(anchor), springConstant(sc), restLength(rl)
{
}

void PointMassAnchoredSpring::updateForce(PointMass* pointMass, real duration)
{
	Vector3 force;
	pointMass->getPosition(&force);
	force -= *anchor;

	// Force magnitude calculation
	real magnitude = force.magnitude();
	magnitude = real_abs(magnitude - restLength);
	magnitude *= springConstant;

	// Calculate final force then apply
	force.normalize();
	force *= -magnitude;
	pointMass->addForce(force);
}