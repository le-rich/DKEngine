#ifndef WORLD_H
#define WORLD_H

#include "Body.h"
#include <vector>

namespace AE86 {
	
	/**
	 * The world represents a simulatino of physics by
	 * keeping track of a set of rigid bodies and updating
	 * them all.
	 */
	class World {
	public:
		typedef std::vector<RigidBody*> RigidBodies;

		World() {
			rigidBodyComponents = std::vector<RigidBody*>();
		}
		
		void addRigidBody(RigidBody* rigidBody);

		RigidBodies getRigidBodies();

		/**
		 * Initializes the world for a simulation frame. This clears
		 * the force and torque accumulators for bodies in the
		 * world. After calling this, the bodies can have their forces
		 * and torques for this frame added.
		 */
		void startFrame();

		// processes physics for the world
		void runPhysics(real duration);

		// ECS-friendly integrate
		void integrate(std::vector<RigidBody*> rigidBodies, real duration);

	protected:
		// holds the rigid bodies being simulated
		RigidBodies rigidBodyComponents;



		void integrate(real duration);


	};
}


#endif // world_h
