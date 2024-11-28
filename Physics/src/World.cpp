
#include "../include/World.h"
#include <iostream>

namespace AE86 {
	void World::startFrame() {
		for (RigidBodies::iterator b = rigidBodyComponents.begin();
			b != rigidBodyComponents.end();
			++b) {

			(*b)->clearAccumulators();
			(*b)->calculateDerivedData();
		}
	}

	void World::integrate(real duration) {
		for (RigidBodies::iterator b = rigidBodyComponents.begin();
			b != rigidBodyComponents.end();
			++b) {

			(*b)->integrate(duration);
		}
	}

	void World::integrate(std::vector<RigidBody*> rigidBodies, real duration) {
		for (RigidBodies::iterator b = rigidBodies.begin();
			b != rigidBodies.end();
			++b) {

			(*b)->integrate(duration);
		}
	}

	void World::runPhysics(real duration) {
		// TODO: add force registry code here.
		// std::cout << "updating physics bodies\n";
		integrate(duration);
	}

	World::RigidBodies World::getRigidBodies() {
		return rigidBodyComponents;
	}

	void World::addRigidBody(RigidBody* rigidBody) {
		rigidBodyComponents.emplace_back(rigidBody);
	}
}