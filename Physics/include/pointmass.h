#ifndef PM_HEADER

#define PM_HEADER

#include "core.h"

namespace AE86 {
	/**
	 * A particle is the simplest object that can be simulated in the
	 * physics system. In reality, it is a point mass as per Newton's
	 * Laws of motion.
	 * Author: Jas Singh
	*/
	class PointMass {
	public:

		void integrate(real duration);

		real getKineticEnergy();

		real getMass() const;
		void setMass(real mass);
		real getInverseMass() const;
		void setInverseMass(const real invMass);
		
		void getPosition(Vector3* position) const;
		Vector3 getPosition() const;
		void setPosition(const Vector3& position);
		void setPosition(const real x, const real y, const real z);

		void getVelocity(Vector3* velocity) const;
		Vector3 getVelocity() const;
		void setVelocity(const Vector3& velocity);
		void setVelocity(const real x, const real y, const real z);

		void getAcceleration(Vector3* acceleration) const;
		Vector3 getAcceleration() const;
		void setAcceleration(const Vector3& acceleration);
		void setAcceleration(const real x, const real y, const real z);

		void clearAccumulator();
		void addForce(const Vector3& force);

	protected:
		/**
		 * Holds the linear positino of the pointMass
		 * in world space.
		 */
		Vector3 position;

		/**
		 * Holds the linear velocity of the pointMass in
		 * world space.
		 */
		Vector3 velocity;

		/**
		 * Holds the acceleration of the pointMass. This value
		 * can be used to set acceleration due to gravity (its primary
		 * use), or any other ocnstant acceleration.
		 */
		Vector3 acceleration;

		/**
		 * Holds the amount of damping applied to linear
		 * motion. Damping is required to remove energy
		 * added through numerical instability in the integrator.
		 * Note on values: if the damping is zero then the velocity
		 * will be reduced to nothing, meaning that the object
		 * couldn't sustain any motion without a force and
		 * would look odd to the player. If you don't want the object
		 * to look like it is experiencing drag, but still want to
		 * use damping to avoid numerical problems, then values slightly
		 * less than 1 are optimal. 0.999 for example.
		 */
		real damping;

		/**
		 * Holds the inverse of the mass of the pointMass. It
		 * is more useful to hold the inverse mass because
		 * integration is simpler, and because in real-time
		 * simulation it is more useful to have objects with
		 * infinite mass (immovable) than zero mass
		 * (completely unstable in numerical simulation).
		 */
		real inverseMass;

		/**
		* Holds the accumulated force to be applied at the next
		* simulation iteration only. This value is zeroed at each
		* integration step.
		*/
		Vector3 forceAccum;
	};
}

#endif