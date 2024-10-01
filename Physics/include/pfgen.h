#include "core.h"
#include "PointMass.h"
#include <vector>
namespace AE86
{
	class PointMassForceGenerator
	{
	public:
		/**
		* Overload this in implementations of the interface to calculate
		* and update the force applied to the given PointMass.
		*/
		virtual void updateForce(PointMass* PointMass, real duration) = 0;
	};
	class PointMassForceRegistry
	{
	protected:
		/**
		* Keeps track of one force generator and the PointMass it
		* applies to.
		*/
		struct PointMassForceRegistration
		{
			PointMass* pointMass;
			PointMassForceGenerator* fg;
		};
		/**
		* Holds the list of registrations.
		*/
		typedef std::vector<PointMassForceRegistration> Registry;
		Registry registrations;
	public:
		/**
		* Registers the given force generator to apply to the
		* given PointMass.
		*/
		void add(PointMass* pointMass, PointMassForceGenerator* fg);
		/**
		* Removes the given registered pair from teh registry.
		* If the pair is not registered, this method will have
		* no effect.
		*/
		void remove(PointMass* pointMass, PointMassForceGenerator* fg);
		/**
		* Clears all registrations from the registry. This will
		* not delete the PointMasss or the force generators
		* themselves, just the records of their connection.
		*/
		void clear();
		/**
		* Calls all the force generators to update the forces of
		* their corresponding PointMasss.
		*/
		void updateForces(real duration);
	};
}
