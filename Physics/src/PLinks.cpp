#include "../include/PLinks.h"

using namespace AE86;

real PointMassLink::currentLength() const
{
	Vector3 relativePos = pointMass[0]->getPosition() - pointMass[1]->getPosition();
	return relativePos.magnitude();
}

unsigned PointMassCable::addContact(PointMassContact* contact, unsigned limit) const
{
	real length = currentLength();

	if (length < maxLength) return 0;

	contact->pointMass[0] = pointMass[0];
	contact->pointMass[1] = pointMass[1];

	Vector3 normal = pointMass[1]->getPosition() - pointMass[0]->getPosition();
	normal.normalize();
	contact->contactNormal = normal;

	contact->penetration = length - maxLength;
	contact->restitution = restitution;

	return 1;
}

unsigned PointMassRod::addContact(PointMassContact* contact, unsigned limit) const
{
	real currentLen = currentLength();

	if (currentLen == length) return 0;

	contact->pointMass[0] = pointMass[0];
	contact->pointMass[1] = pointMass[1];

	Vector3 normal = pointMass[1]->getPosition() - pointMass[0]->getPosition();
	normal.normalize();

	if (currentLen > length)
	{
		contact->contactNormal = normal;
		contact->penetration = currentLen - length;
	}
	else
	{
		contact->contactNormal = normal * -1;
		contact->penetration = length - currentLen;
	}

	contact->restitution = 0;

	return 1;
}

real PointMassConstraint::currentLength() const
{
	Vector3 relativePos = pointMass->getPosition() - anchor;
	return relativePos.magnitude();
}

unsigned PointMassCableConstraint::addContact(PointMassContact* contact, unsigned limit) const
{
	real length = currentLength();

	if (length < maxLength)
	{
		return 0;
	}

	contact->pointMass[0] = pointMass;
	contact->pointMass[1] = 0;

	Vector3 normal = anchor - pointMass->getPosition();
	normal.normalize();
	contact->contactNormal = normal;

	contact->penetration = length - maxLength;
	contact->restitution = restitution;

	return 1;
}

unsigned PointMassRodConstraint::addContact(PointMassContact* contact, unsigned limit) const
{
	real currentLen = currentLength();

	if (currentLen == length)
	{
		return 0;
	}

	contact->pointMass[0] = pointMass;
	contact->pointMass[1] = 0;

	Vector3 normal = anchor - pointMass->getPosition();
	normal.normalize();

	if (currentLen > length) {
		contact->contactNormal = normal;
		contact->penetration = currentLen - length;
	}
	else {
		contact->contactNormal = normal * -1;
		contact->penetration = length - currentLen;
	}

	contact->restitution = 0;

	return 1;
}