#include "../include/BVH.h"

namespace AE86 {
	template<class BoundingVolume>
	bool BVHNode<BoundingVolume>::overlaps(
		const BVHNode<BoundingVolume>* other) const {
		return volume->overlaps(other->volume);
	}

	
	template<class BoundingVolume>
	void BVHNode<BoundingVolume>::insert(RigidBody* newBody,
		const BoundingVolume& newVolume) {
		 
		if (isLeaf()) {
			children[0] = new BVHNode<BoundingVolume>(
				this, volume, body
			);

			children[1] = new BVHNode<BoundingVolume>(
				this, newVolume, newBody
			);

			this->body = NULL;

			recalculateBoundingVolume();
		}
		else {
			if (children[0]->volume.getGrowth(newVolume) < children[1]->volume.getGrowth(newVolume)) {
				children[0]->insert(newBody, newVolume);
			}
			else {
				children[1]->insert(newBody, newVolume);
			}
		}
	}

	template<class BoundingVolume>
	BVHNode<BoundingVolume>::~BVHNode() {
		// no parent then ignore sibling
		if (parent) {
			// find sibling
			BVHNode<BoundingVolume>* sibling;
			if (parent->children[0] == this)
				sibling = parent->children[1];
			else
				sibling = parent->children[0];

			parent->volume = sibling->volume;
			parent->body = sibling->body;
			parent->children[0] = sibling->children[0];
			parent->children[1] = sibling->children[1];

			sibling->parent = NULL;
			sibling->body = NULL;
			sibling->children[0] = NULL;
			sibling->children[1] = NULL;
			delete sibling;

			parent->recalculateBoundingVolume();
		}

		// delete this node's children
		if (children[0]) {
			children[0]->parent = NULL;
			delete children[0];
		}
		if (children[1]) {
			children[1]->parent = NULL;
			delete children[1];
		}
	}

	template<class BoundingVolume>
	void BVHNode<BoundingVolume>::recalculateBoundingVolume(bool recurse) {
		if (isLeaf()) 
			return;

		volume = BoundingVolume(
			children[0]->volume,
			children[1]->volume
		);

		if (parent)
			parent->recalculateBoundingVolume(true);
	}

	template<class BoundingVolume>
	unsigned BVHNode<BoundingVolume>::getPossibleContacts(
		PossibleContact* contacts,
		unsigned limit) const {

		// at our limit or we're leaf (nothing to collide with
		if (isLeaf() || limit == 0)
			return 0;

		// both children tested for collision
		return children[0]->getPotentialContactsWith(children[1], contacts, limit);
	}

	template<class BoundingVolume>
	unsigned BVHNode<BoundingVolume>::getPossibleContactsWith(
		const BVHNode<BoundingVolume>* other,
		PossibleContact* contacts,
		unsigned limit) const {

		// if there's no overlap or we're at our limit - return 0.
		if (!overlaps(other) || limit == 0)
			return 0;

		// if both of the tested bvhns are leaves, then possible collision
		if (isLeaf() && other->isLeaf()) {
			contacts->bodies[0] = body;
			contacts->bodies[1] = other->body;
			return 1;
		}

		if (other->isLeaf() ||
			(!isLeaf() && volume->getSize() >= other->volume->getSize())) {

			unsigned count = children[0]->getPossibleContactsWith(
				other, contacts, limit
			);

			if (limit > count) {
				return count + children[1]->getPotentialContactsWith(
					other, 
					contacts + count,
					limit - count);
			}
			else {
				return count;
			}
		}
	}

}