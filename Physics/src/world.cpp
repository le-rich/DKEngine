
#include "../include/World.h"
#include <iostream>

namespace AE86 {
	void World::startFrame() {
		for (RigidBodies::iterator b = bodies.begin();
			b != bodies.end();
			++b) {

			(*b)->clearAccumulators();
			(*b)->calculateDerivedData();
		}
	}

	void World::integrate(real duration) {
		for (RigidBodies::iterator b = bodies.begin();
			b != bodies.end();
			++b) {

			(*b)->integrate(duration);
		}
	}

	void World::runPhysics(real duration) {
		// TODO: add force registry code here.
		std::cout << "updating physics bodies\n";
		integrate(duration);
	}

	World::RigidBodies World::getRigidBodies() {
		return bodies;
	}

	void World::addRigidBody(RigidBody* rigidBody) {
		bodies.emplace_back(rigidBody);
	}
}