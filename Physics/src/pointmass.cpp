#include <assert.h>
#include "../include/pointmass.h"

using namespace AE86;

// ------------ Misc ------------

void PointMass::integrate(real duration)
{

	// don't integrate if the pointMass has infinite mass
	if (inverseMass <= ((real) 0.0)) return;

	assert(duration > 0.0);

	// update linear position
	position.addScaledVector(velocity, duration);

	// work out the acceleration from the force.
	Vector3 resultingAcc = acceleration;

	resultingAcc.addScaledVector(forceAccum, inverseMass);

	// update linear velocity from acceleration
	velocity.addScaledVector(resultingAcc, duration);

	// impose drag/damping
	velocity *= real_pow(damping, duration);

	// Clear the forces.
	clearAccumulator();
}

real PointMass::getKineticEnergy()
{
	return ((real)0.5) * (((real)1.0) / inverseMass) * velocity.magnitude();
}

// ------------ Mass ------------

real PointMass::getMass() const
{
	if (PointMass::inverseMass <= 0.0)
	{
		return REAL_MAX;
	}
	else
	{
		return ((real)1.0) / inverseMass;
	}
}

void PointMass::setMass(real mass)
{
	inverseMass = ((real)1.0) / mass;
}

real PointMass::getInverseMass() const
{
	return inverseMass;
}

void PointMass::setInverseMass(real invMass)
{
	inverseMass = invMass;
}

// ------------ Position ------------

void PointMass::getPosition(Vector3* outPosition) const
{
	*outPosition = PointMass::position;
}

Vector3 PointMass::getPosition() const
{
	return PointMass::position;
}

void PointMass::setPosition(const Vector3& position)
{
	PointMass::position = position;
}

void PointMass::setPosition(const real x, const real y, const real z)
{
	PointMass::position.x = x;
	PointMass::position.y = y;
	PointMass::position.z = z;
}

// ------------ Velocity ------------

void PointMass::getVelocity(Vector3* outVelocity) const
{
	*outVelocity = PointMass::velocity;
}

Vector3 PointMass::getVelocity() const
{
	return PointMass::velocity;
}

void PointMass::setVelocity(const Vector3& velocity)
{
	PointMass::velocity = velocity;
}

void PointMass::setVelocity(const real x, const real y, const real z)
{
	PointMass::velocity.x = x;
	PointMass::velocity.y = y;
	PointMass::velocity.z = z;
}

// ------------ Acceleration ------------

void PointMass::getAcceleration(Vector3* outAcceleration) const
{
	*outAcceleration = PointMass::acceleration;
}

Vector3 PointMass::getAcceleration() const
{
	return PointMass::acceleration;
}

void PointMass::setAcceleration(const Vector3& acceleration)
{
	PointMass::acceleration = acceleration;
}

void PointMass::setAcceleration(const real x, const real y, const real z)
{
	PointMass::acceleration.x = x;
	PointMass::acceleration.y = y;
	PointMass::acceleration.z = z;
}

// ------------ Forces ------------

void PointMass::clearAccumulator() {
	forceAccum.clear();
}

void PointMass::addForce(const Vector3& force)
{
	forceAccum += force;
}