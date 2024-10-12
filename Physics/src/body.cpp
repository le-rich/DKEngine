#include "../include/body.h"

namespace AE86 {
	
	/**
	 * A function that creates a transform matrix
	 * from a position and orientation.
	 */
	static inline void _calculateTransformMatrix(
		Matrix4 &transformMatrix, const Vector3 &position,
		Quaternion &orientation) {

        transformMatrix.data[0] = 1 - 2 * orientation.j * orientation.j -
            2 * orientation.k * orientation.k;
        transformMatrix.data[1] = 2 * orientation.i * orientation.j -
            2 * orientation.r * orientation.k;
        transformMatrix.data[2] = 2 * orientation.i * orientation.k +
            2 * orientation.r * orientation.j;
        transformMatrix.data[3] = position.x;

        transformMatrix.data[4] = 2 * orientation.i * orientation.j +
            2 * orientation.r * orientation.k;
        transformMatrix.data[5] = 1 - 2 * orientation.i * orientation.i -
            2 * orientation.k * orientation.k;
        transformMatrix.data[6] = 2 * orientation.j * orientation.k -
            2 * orientation.r * orientation.i;
        transformMatrix.data[7] = position.y;

        transformMatrix.data[8] = 2 * orientation.i * orientation.k -
            2 * orientation.r * orientation.j;
        transformMatrix.data[9] = 2 * orientation.j * orientation.k +
            2 * orientation.r * orientation.i;
        transformMatrix.data[10] = 1 - 2 * orientation.i * orientation.i -
            2 * orientation.j * orientation.j;
        transformMatrix.data[11] = position.z;
	}

    void RigidBody::calculateDerivedData() {
        orientation.normalize();

        // Calculate the transform matrix for the body.
        _calculateTransformMatrix(transformMatrix, position, orientation);
    }

    void RigidBody::setInertiaTensor(const Matrix3& inertiaTensor) {
        inverseInertiaTensor.setInverse(inertiaTensor);
    }
}