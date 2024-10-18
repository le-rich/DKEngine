#ifndef FGEN_HEADER

#define FGEN_HEADER

#include "body.h"
#include "pfgen.h"

namespace AE86 {
	/** 
	 * A force generator, can apply a force to one or more bodies.
	 */
	class ForceGenerator {
	public:
		/**
		 * Calculates and updates the ofrce applied to a given rb.
		 */
		virtual void updateForce(RigidBody* body, real duration) = 0;
	};

	/**
	 * A force generator for gravitational force.
	 * One instance applies to none or many rigid bodies.
	 */
	class Gravity : public ForceGenerator {
	private:
		Vector3& gravity;

	public:
		// constructor taking gravity acceleration
		Gravity(Vector3& gravityAccel);

		// gravitational force is paplied to the given rigid body.
		virtual void updateForce(RigidBody* body, real duration);
	};

	/**
	 * A force generator that applies a spring force.
	 */
	class Spring :public ForceGenerator {
		/**
		* The point of connection of the spring in local
		* coordinates.
		*/
		Vector3 connectionPoint;

		/**
		* The point of connection of the spring to the other object
		* in that object’s local coordinates.
		*/
		Vector3 otherConnectionPoint;

		/** The particle at the other end of the spring. */
		RigidBody* other;

		/** Holds the spring constant. */
		real springConstant;

		/** Holds the rest length of the spring. */
		real restLength;

	public:

		/** Creates a new spring with the given parameters. */
		Spring(const Vector3& localConnectionPt,
			RigidBody* other,
			const Vector3& otherConnectionPt,
			real springConstant,
			real restLength);

		/** Applies the spring force to the given rigid body. */
		virtual void updateForce(RigidBody* body, real duration);
	};
}


#endif