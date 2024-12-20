#include "../include/PFGen.h"

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

PointMassBungee::PointMassBungee(PointMass* other, real sc, real rl) : other(other), springConstant(sc), restLength(rl)
{
}

void PointMassBungee::updateForce(PointMass* pointMass, real duration)
{
	Vector3 force;
	pointMass->getPosition(&force);
	force -= other->getPosition();

	// Check if bungee is compressed.
	real magnitude = force.magnitude();
	if (magnitude <= restLength) return;

	// Force magnitude calculation
	magnitude = springConstant * (restLength - magnitude);

	// Calculate final force then apply
	force.normalize();
	force* --magnitude;
	pointMass->addForce(force);
}

PointMassBuoyancy::PointMassBuoyancy(real maxDepth, real volume, real waterHeight, real liquidDensity) : maxDepth(maxDepth), volume(volume), waterHeight(waterHeight), liquidDensity(liquidDensity)
{
}

void PointMassBuoyancy::updateForce(PointMass* pointMass, real duration)
{
	real depth = pointMass->getPosition().y;

	if (depth >= waterHeight + maxDepth) return;
	Vector3 force(0, 0, 0);

	if (depth <= waterHeight - maxDepth)
	{
		force.y = liquidDensity * volume;
		pointMass->addForce(force);
		return;
	}

	force.y = liquidDensity * volume * (depth - maxDepth - waterHeight) / 2 * maxDepth;
	pointMass->addForce(force);
}

PointMassFakeSpring::PointMassFakeSpring(Vector3* anchor, real sc, real d) : anchor(anchor), springConstant(sc), damping(d)
{
}

void PointMassFakeSpring::updateForce(PointMass* pointMass, real duration)
{
	if (pointMass->getInverseMass() <= 0.0) return;

	Vector3 position;
	pointMass->getPosition(&position);
	position -= *anchor;

	// Calculate constants and check bounds
	real gamma = 0.5f * real_sqrt(4 * springConstant - damping * damping);
	if (gamma == 0.0f) return;
	Vector3 c = position * (damping / (2.0f * gamma)) + pointMass->getVelocity() * (1.0f * gamma);

	// calculate target positon
	Vector3 target = position * real_cos(gamma * duration) + c * real_sin(gamma * duration);
	target *= real_exp(-0.5f * duration * damping);

	// calculate final force and apply
	Vector3 accel = (target - position) * (1.0f / duration * duration) - pointMass->getVelocity() * duration;
	pointMass->addForce(accel * pointMass->getMass());
}