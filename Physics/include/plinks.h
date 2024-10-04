#ifndef PLINK_HEADER

#define PLINK_HEADER

#include "pointmass.h"
#include "pcontacts.h"

namespace AE86
{
	class PointMassLink : PointMassContactGenerator
	{
	public:

		PointMass* pointMass[2];

	protected:

		real currentLength() const;

	public:

		virtual unsigned addContact(PointMassContact* contact, unsigned limit) const = 0;
	};

	class PointMassCable : public PointMassLink
	{
	public:

		real maxLength;

		real restitution;

	public:

		virtual unsigned addContact(PointMassContact* contact, unsigned limit) const;
	};

	class PointMassRod : public PointMassLink
	{
	public:

		real length;

	public:

		// real currentLength() const;  // Length is constant, ideally.

		virtual unsigned addContact(PointMassContact* contact, unsigned limit) const;
	};
}

#endif