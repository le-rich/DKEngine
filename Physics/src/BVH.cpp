#include "../include/BVH.h"

namespace AE86 {
	template<class BoundingVolume>
	bool BVHNode<BoundingVolume>::overlaps(
		const BVHNode<BoundingVolume>* other) const {
		return volume->overlaps(other->volume);
	}

	
	template<class BoundingVolume>
	void BVHNode<BoundingVolume>::insert(RigidBody* body,
		const BoundingVolume& volume) {


	}
}