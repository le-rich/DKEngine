#include "../include/BoundingVolume.h"

namespace AE86 {
    BoundingSphere::BoundingSphere(const Vector3& centre, real radius)
        : centre(centre), radius(radius)
    {
    }

    BoundingSphere::BoundingSphere(const BoundingSphere& one, const BoundingSphere& two) {
        Vector3 centreAToCentreB = two.centre - one.centre;
        real distance = centreAToCentreB.squareMagnitude();
        real radiusDiff = two.radius - one.radius;

        // below checks if one sphere completely dwarfs the other
        // if so, we set this bounding sphere's properties to the larger 
        // one's
        if (radiusDiff * radiusDiff >= distance) {
            // check which one is bigger
            if (one.radius > two.radius) {
                centre = one.centre;
                radius = one.radius;
            }
            else {
                centre = two.centre;
                radius = two.radius;
            }

            return; // we are done here
        }


        // if we're here, one sphere doesn't dwarf the other's.
        distance = real_sqrt(distance);

        radius = (distance + one.radius + two.radius) * ((real)0.5);

        centre = one.centre;
        
        if (distance > 0)
            centre += centreAToCentreB * ((radius - one.radius) / distance);
    }


    bool BoundingSphere::overlaps(const BoundingSphere* other) const {
        real distance = (centre - other->centre).squareMagnitude();

        // simple squared distance < squared radii
        return distance < (radius + other->radius) * (radius + other->radius);
    }

    real BoundingSphere::getGrowth(const BoundingSphere& other) const {
        BoundingSphere newSphere(*this, other);
        return newSphere.radius * newSphere.radius - radius * radius;
    }
}
