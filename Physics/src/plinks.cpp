#include "../include/plinks.h"

using namespace AE86;

real PointMassLink::currentLength() const
{
	Vector3 relativePos = pointMass[0]->getPosition() - pointMass[1]->getPosition();
	return relativePos.magnitude();
}