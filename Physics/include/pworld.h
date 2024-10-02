#ifndef PWORLD_HEADER

#define PWORLD_HEADER

#include "pfgen.h"
#include "plinks.h"

namespace AE86
{
	class PointMassWorld
	{
	public:

		typedef std::vector<PointMass*> PointMasss;

		typedef std::vector<PointMassContactGenerator*> ContactGenerators;

	protected:

		PointMasss pointMasss;

		bool calculateIterations;

		PointMassForceRegistry registry;

		PointMassContactResolver resolver;

		ContactGenerators contactGenerators;

		PointMassContact* contacts;

		unsigned maxContacts;

	public:

		PointMassWorld(unsigned maxContacts, unsigned iterations = 0);

		~PointMassWorld();

		unsigned generateContacts();

		void integrate(real duration);

		void runPhysics(real duration);

		void startFrame();
	};
}

#endif