#ifndef BOUNDING_VOLUME_H
#define BOUNDING_VOLUME_H

#include <vector>
#include <cstddef>
#include "Core.h"

// this file should contain the definitions for all bounding volumes
// added to the engine. TODO: refactor AABBs to be here.
namespace AE86 {

	// A Bounding Sphere
	struct BoundingSphere {
		Vector3 centre;
		real radius;

	public:
		/* Creates a bounding sphere with the specified centre and radius */
		BoundingSphere(const Vector3 &centre, real radius);

		/* Creates a bounding sphere that overlaps the two passed in, useful
		 * for Bounding Volume Hierarchies. 
		 */
		BoundingSphere(const BoundingSphere& one, const BoundingSphere& two);


        /* Checks if the bounding sphere overlaps with the passed in one */
        bool overlaps(const BoundingSphere* other) const;

        /**
         * How much the bounding sphere needs to grow (in surface area) 
         * to overlap the passed in bounding sphere.
         */
        real getGrowth(const BoundingSphere& other) const;

        /**
         * Returns the volume of this bounding volume. This is used
         * to calculate how to recurse into the bounding volume tree.
         * For a bounding sphere it is a simple calculation.
         */
        real getSize() const
        {
            return ((real)1.333333) * R_PI * radius * radius * radius;
        }
	};
}


#endif // BOUNDING_VOLUME_H