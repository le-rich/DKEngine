#include "../include/pcontacts.h"

using namespace AE86;

void PointMassContact::resolve(real duration)
{
	resolveVelocity(duration);
	resolveInterpenetration(duration);
}

real PointMassContact::calculateSeparatingVelocity() const
{
	Vector3 relativeVelocity = pointMass[0]->getVelocity();
	if (pointMass[1]) relativeVelocity -= pointMass[1]->getVelocity();
	return relativeVelocity * contactNormal;
}

void PointMassContact::resolveVelocity(real duration)
{
	// find separating velocity
	real separatingVelocity = calculateSeparatingVelocity();

	// check if needs resolving, exit if not necessary
	if (separatingVelocity > 0) return;

	real newSepVelocity = -separatingVelocity * restitution;

	Vector3 accCausedVelocity = pointMass[0]->getAcceleration();
	if (pointMass[1]) accCausedVelocity -= pointMass[1]->getAcceleration();

	real accCausedSepVelocity = accCausedVelocity * contactNormal * duration;

	if (accCausedSepVelocity < 0)
	{
		newSepVelocity += restitution * accCausedSepVelocity;

		if (newSepVelocity < 0) newSepVelocity = 0;
	}

	real deltaVelocity = newSepVelocity - separatingVelocity;


	real totalInverseMass = pointMass[0]->getInverseMass();  // helper function? [getTotalIM()]
	if (pointMass[1]) totalInverseMass += pointMass[1]->getInverseMass();  // helper function? [getTotalIM()]

	if (totalInverseMass <= 0) return;  // helper function? [getTotalIM()]

	real impulse = deltaVelocity / totalInverseMass;

	Vector3 impulsePerIMass = contactNormal * impulse;

	pointMass[0]->setVelocity(pointMass[0]->getVelocity() + impulsePerIMass * pointMass[0]->getInverseMass());

	if (pointMass[1])
	{
		pointMass[1]->setVelocity(pointMass[1]->getVelocity() + impulsePerIMass * pointMass[1]->getInverseMass());
	}
}

void PointMassContact::resolveInterpenetration(real duration)
{
	if (penetration <= 0) return;

	real totalInverseMass = pointMass[0]->getInverseMass();  // helper function? [getTotalIM()]
	if (pointMass[1]) totalInverseMass += pointMass[1]->getInverseMass();  // helper function? [getTotalIM()]

	if (totalInverseMass <= 0) return;  // helper function? [getTotalIM()]

	Vector3 movePerIMass = contactNormal * (-penetration / totalInverseMass);

	pointMass[0]->setPosition(pointMass[0]->getPosition() + movePerIMass * pointMass[0]->getInverseMass());

	if (pointMass[1])
	{
		pointMass[1]->setPosition(pointMass[1]->getPosition() + movePerIMass * pointMass[1]->getInverseMass());
	}
}

PointMassContactResolver::PointMassContactResolver(unsigned iterations) : iterations(iterations)
{
}

void PointMassContactResolver::setIterations(unsigned iterations)
{
	PointMassContactResolver::iterations = iterations;
}

void PointMassContactResolver::resolveContacts(PointMassContact* contactArray, unsigned numContacts, real duration)
{
	iterationsUsed = 0;

	while (iterationsUsed < iterations)
	{
		real max = 0;
		unsigned maxIndex = numContacts;

		for (unsigned i = 0; i < numContacts; i++)
		{
			real sepVel = contactArray[i].calculateSeparatingVelocity();
			if (sepVel < max)
			{
				max = sepVel;
				maxIndex = i;
			}
		}

		contactArray[maxIndex].resolve(duration);
		iterationsUsed++;
	}
}