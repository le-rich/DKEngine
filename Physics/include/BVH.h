#ifndef BOUNDING_VOLUME_HIERARCHY_H
#define BOUNDING_VOLUME_HIERARCHY_H

#include <vector>
#include <cstddef>
#include "Body.h"

namespace AE86 {
	struct PossibleContact {
		// rbs that might've collided
		RigidBody* bodies[2];
	};


	/**
	 * An insertion-based bounding volume hierarchy as a binary tree
	 */
	template<class BoundingVolume>
	class BVHNode {
	public:
		BVHNode* children[2];
		
		// the collision object and rigidbody
		BoundingVolume volume;
		RigidBody* body;

		BVHNode* parent;

		// creates a new node in the hierarchy with the given parent node
		BVHNode(BVHNode* parent, const BoundingVolumeClass& volume,
			RigidBody* body = NULL)
			: parent(parent), volume(volume), body(body)
		{
			children[0] = children[1] = NULL;
		}

		bool isLeaf() const {
			return (body != NULL);
		}

		/*
		 * Checks for potential contacts from this node down the tree,
		 * writing them to the passed in array. Stops if it reaches limit
		 * number of collisions. Returns an unsigned int containing the number
		 * of potential collisions detected.
		 */
		unsigned getPotentialContacts(PotentialContact* contacts,
			unsigned limit) const;

		/*
		 * Inserts rigidbody and bounding volume into the hierarchy.
		 * Can create further bounding volumes as a result.
		 */
		void insert(RigidBody* body, const BoundingVolume& volume);

		/*
		 * Deletes the node, also removing it from the hierarchy.
		 */
		~BVHNode();

	protected:
		/*
		 * Checks for overlapping nodes in the hierarchy
		 */
		bool overlaps(const BVHNode<BoundingVolume>* other) const;

		/* 
		 * checks for potential contacts between this node and passed in node
		 * writing them to the contacts array. It terminates if it reaches
		 * limit number of contacts, returns an unsigned int of the number of
		 * potential contacts detected.
		 */
		unsigned getPotentialContactsWith(
			const BVHNode<BoundingVolume>* other,
			PotentialContact* contacts,
			unsigned limit) const;

		/*
		 * Recalculates the bounding volume based on the ones 
		 * of its children. NOTE: do not use on leaf nodes.
		 */
		void recalculateBoundingVolume(bool recurse = true);
	};

}


#endif // BOUNDING_VOLUME_HIERARCHY_H