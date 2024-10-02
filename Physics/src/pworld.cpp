#include "../include/pworld.h"

using namespace AE86;

PointMassWorld::PointMassWorld(unsigned maxContacts, unsigned iterations) : resolver(iterations), maxContacts(maxContacts)
{
	contacts = new PointMassContact[maxContacts];
	calculateIterations = (iterations == 0);

}

PointMassWorld::~PointMassWorld()
{
	delete[] contacts;
}