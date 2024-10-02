#ifndef PFGEN_HEADER

#define PFGEN_HEADER

#include "core.h"
#include "pointmass.h"
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

	class PointMassGravity : public PointMassForceGenerator
	{
		/** Holds the acceleration due to gravity. */
		Vector3 gravity;
	public:
		/** Creates the generator with the given acceleration. */
		PointMassGravity(const Vector3& gravity);
		/** Applies the gravitational force to the given pointMass. */
		virtual void updateForce(PointMass* pointMass, real duration);
	};

    class PointMassDrag : public PointMassForceGenerator
	{
		/** Holds the velocity drag coefficient. */
		real k1;
		/** Holds the velocity squared drag coefficient. */
		real k2;
	public:
		/** Creates the generator with the given coefficients. */
		PointMassDrag(real k1, real k2);
		/** Applies the drag force to the given pointMass. */
		virtual void updateForce(PointMass* pointMass, real duration);
	};

	class PointMassSpring : public PointMassForceGenerator
	{

		PointMass* other;

		real springConstant;

		real restLength;

	public:

		PointMassSpring(PointMass* other, real springConstant, real restLength);

		virtual void updateForce(PointMass* pointMass, real duration);
	};

	class PointMassAnchoredSpring : public PointMassForceGenerator
	{
		Vector3* anchor;

		real springConstant;

		real restLength;

	public:

		PointMassAnchoredSpring(Vector3* anchor, real springConstant, real restLength);

		virtual void updateForce(PointMass* pointMass, real duration);
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
		* 
		* NOT DEFINED
		*/
		void remove(PointMass* pointMass, PointMassForceGenerator* fg);
		/**
		* Clears all registrations from the registry. This will
		* not delete the PointMasss or the force generators
		* themselves, just the records of their connection.
		* 
		* NOT DEFINED
		*/
		void clear();
		/**
		* Calls all the force generators to update the forces of
		* their corresponding PointMasss.
		*/
		void updateForces(real duration);
	};
}

#endif