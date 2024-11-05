#ifndef PCONTACT_HEADER

#define PCONTACT_HEADER

#include "pointmass.h"

namespace AE86
{
	class PointMassContact
	{

		friend class PointMassContactResolver;

	public:
		// pointMasss in contact
		// second can be null
		PointMass* pointMass[2];

		// normal restitution coefficient
		real restitution;

		// direction of contact normal
		Vector3 contactNormal;

		// penetration depth
		real penetration;

		Vector3 pointMassMovement[2];

	protected:

		void resolve(real duration);

		real calculateSeparatingVelocity() const;

	private:

		void resolveVelocity(real duration);

		void resolveInterpenetration(real duration);
	};

	class PointMassContactResolver
	{
	protected:
		// num iterations allowed
		unsigned iterations;

		// num used iterations
		unsigned iterationsUsed;

	public:

		PointMassContactResolver(unsigned iterations);

		void setIterations(unsigned iterations);

		void resolveContacts(PointMassContact* contactArray, unsigned numContacts, real duration);
	};

	class PointMassContactGenerator
	{
	public:

		virtual unsigned addContact(PointMassContact* contact, unsigned limit) const = 0;
	};
}

#endif