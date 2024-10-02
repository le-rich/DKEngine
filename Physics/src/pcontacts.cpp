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