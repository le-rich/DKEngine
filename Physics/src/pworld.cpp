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

unsigned PointMassWorld::generateContacts()
{
	unsigned limit = maxContacts;
	PointMassContact* nextContact = contacts;

	for (ContactGenerators::iterator g = contactGenerators.begin(); g != contactGenerators.end(); g++)
	{
		unsigned used = (*g)->addContact(nextContact, limit);
		limit -= used;
		nextContact += used;

		if (limit <= 0) break;
	}

	return maxContacts - limit;
}

void PointMassWorld::integrate(real duration)
{
	for (PointMasss::iterator p = pointMasss.begin(); p != pointMasss.end(); p++)
	{
		(*p)->integrate(duration);
	}
}

void PointMassWorld::runPhysics(real duration)
{
	registry.updateForces(duration);

	integrate(duration);

	unsigned usedContacts = generateContacts();

	if (usedContacts)
	{
		if (calculateIterations) resolver.setIterations(usedContacts * 2);
		resolver.resolveContacts(contacts, usedContacts, duration);
	}
}

void PointMassWorld::startFrame()
{
	for (PointMasss::iterator p = pointMasss.begin(); p != pointMasss.end(); p++)
	{
		(*p)->clearAccumulator();
	}
}