#include "../include/body.h"
#include <cassert>

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


    /**
     * intertia tensor transform by a quaternion,
     * change of basis transform applied treating the 4x3 transformation
     * matrix as a 3x3 one to savec ompute time.
     */
    static inline void _transformInertiaTensor(Matrix3& iitWorld,
        const Quaternion& q,
        const Matrix3& iitBody,
        const Matrix4& rotmat)
    {
        real t4 = rotmat.data[0] * iitBody.data[0] +
            rotmat.data[1] * iitBody.data[3] +
            rotmat.data[2] * iitBody.data[6];
        real t9 = rotmat.data[0] * iitBody.data[1] +
            rotmat.data[1] * iitBody.data[4] +
            rotmat.data[2] * iitBody.data[7];
        real t14 = rotmat.data[0] * iitBody.data[2] +
            rotmat.data[1] * iitBody.data[5] +
            rotmat.data[2] * iitBody.data[8];
        real t28 = rotmat.data[4] * iitBody.data[0] +
            rotmat.data[5] * iitBody.data[3] +
            rotmat.data[6] * iitBody.data[6];
        real t33 = rotmat.data[4] * iitBody.data[1] +
            rotmat.data[5] * iitBody.data[4] +
            rotmat.data[6] * iitBody.data[7];
        real t38 = rotmat.data[4] * iitBody.data[2] +
            rotmat.data[5] * iitBody.data[5] +
            rotmat.data[6] * iitBody.data[8];
        real t52 = rotmat.data[8] * iitBody.data[0] +
            rotmat.data[9] * iitBody.data[3] +
            rotmat.data[10] * iitBody.data[6];
        real t57 = rotmat.data[8] * iitBody.data[1] +
            rotmat.data[9] * iitBody.data[4] +
            rotmat.data[10] * iitBody.data[7];
        real t62 = rotmat.data[8] * iitBody.data[2] +
            rotmat.data[9] * iitBody.data[5] +
            rotmat.data[10] * iitBody.data[8];

        iitWorld.data[0] = t4 * rotmat.data[0] +
            t9 * rotmat.data[1] +
            t14 * rotmat.data[2];
        iitWorld.data[1] = t4 * rotmat.data[4] +
            t9 * rotmat.data[5] +
            t14 * rotmat.data[6];
        iitWorld.data[2] = t4 * rotmat.data[8] +
            t9 * rotmat.data[9] +
            t14 * rotmat.data[10];
        iitWorld.data[3] = t28 * rotmat.data[0] +
            t33 * rotmat.data[1] +
            t38 * rotmat.data[2];
        iitWorld.data[4] = t28 * rotmat.data[4] +
            t33 * rotmat.data[5] +
            t38 * rotmat.data[6];
        iitWorld.data[5] = t28 * rotmat.data[8] +
            t33 * rotmat.data[9] +
            t38 * rotmat.data[10];
        iitWorld.data[6] = t52 * rotmat.data[0] +
            t57 * rotmat.data[1] +
            t62 * rotmat.data[2];
        iitWorld.data[7] = t52 * rotmat.data[4] +
            t57 * rotmat.data[5] +
            t62 * rotmat.data[6];
        iitWorld.data[8] = t52 * rotmat.data[8] +
            t57 * rotmat.data[9] +
            t62 * rotmat.data[10];
    }

    void RigidBody::setAwake(const bool awake) {
        if (awake) {
            isAwake = true;

            // TODO: add delay so sleep feels more graceful
        }
        else {
            isAwake = false;
            velocity.clear();
            rotation.clear();
        }
    }

    void RigidBody::calculateDerivedData() {
        orientation.normalize();

        // calculate the transform matrix for the body.
        _calculateTransformMatrix(transformMatrix, position, orientation);

        // calculate the inertia tensor in world space
        _transformInertiaTensor(inverseInertiaTensorWorld,
            orientation,
            inverseInertiaTensor,
            transformMatrix);
    }

    void RigidBody::setInertiaTensor(const Matrix3& inertiaTensor) {
        inverseInertiaTensor.setInverse(inertiaTensor);
    }
    
    Vector3 RigidBody::getPointInLocalSpace(const Vector3& point) const {
        return transformMatrix.transformInverse(point);
    }

    Vector3 RigidBody::getPointInWorldSpace(const Vector3& point) const {
        return transformMatrix.transform(point);
    }

    void RigidBody::addForce(const Vector3& force) {
        forceAccum += force;
        isAwake = true;
    }

    void RigidBody::addForceAtBodyPoint(const Vector3& force,
        const Vector3& point) {

        // transform to world coords
        Vector3 pt = getPointInWorldSpace(point);

        // apply world coordinate force
        addForceAtPoint(force, pt);
    }

    void RigidBody::addForceAtPoint(const Vector3& force,
        const Vector3& point) {
        
        // vector from force-applied coord to centre of mass
        Vector3 pt = point - position;

        // d'alembert's principle on force and torque
        forceAccum += force;
        torqueAccum += pt % force; // overloaded cross prod.

        isAwake = true;
    }

    void RigidBody::clearAccumulators() {
        forceAccum.clear();
        torqueAccum.clear();
    }

    void RigidBody::integrate(real duration) {
        // linear acceleration
        lastFrameAcceleration = acceleration;
        lastFrameAcceleration.addScaledVector(forceAccum, inverseMass);

        // angular acceleration
        Vector3 angularAcceleration =
            inverseInertiaTensorWorld.transform(torqueAccum);

        // velocity update
        velocity.addScaledVector(lastFrameAcceleration, duration);

        // update angular velocity
        rotation.addScaledVector(angularAcceleration, duration);


        // drag
        velocity *= real_pow(linearDamping, duration);
        rotation *= real_pow(angularDamping, duration);

        // update linear position
        position.addScaledVector(velocity, duration);

        // update angular position
        orientation.addScaledVector(rotation, duration);

        // update data with new state
        calculateDerivedData();

        // clear forces applied
        clearAccumulators();
    }

    real RigidBody::getMass() const
    {
        if (inverseMass == 0) {
            return REAL_MAX;
        }
        else {
            return ((real)1.0) / inverseMass;
        }
    }

    bool RigidBody::hasFiniteMass() const {
        return inverseMass >= 0.0f;
    }

    void RigidBody::setMass(const real mass)
    {
        assert(mass != 0); // can't divide by zero.
        RigidBody::inverseMass = ((real)1.0) / mass;
    }

    void RigidBody::setInverseMass(const real inverseMass)
    {
        RigidBody::inverseMass = inverseMass;
    }

    real RigidBody::getInverseMass() const
    {
        return inverseMass;
    }

    void RigidBody::setLinearDamping(real damping) {
        linearDamping = damping;
    }

    real RigidBody::getLinearDamping() const {
        return linearDamping;
    }

    void RigidBody::setAngularDamping(real damping) {
        angularDamping = damping;
    }

    real RigidBody::getAngularDamping() const {
        return angularDamping;
    }

    void RigidBody::setPosition(const Vector3& position)
    {
        RigidBody::position = position;
    }

    void RigidBody::getPosition(Vector3* out) const
    {
        *out = RigidBody::position;
    }

    Vector3 RigidBody::getPosition() const
    {
        return position;
    }

    void RigidBody::setOrientation(const Quaternion& orientation)
    {
        RigidBody::orientation = orientation;
        RigidBody::orientation.normalize();
    }

    void RigidBody::getOrientation(Quaternion* out) const
    {
        *out = RigidBody::orientation;
    }

    Quaternion RigidBody::getOrientation() const
    {
        return orientation;
    }

    void RigidBody::setVelocity(const Vector3& velocity)
    {
        RigidBody::velocity = velocity;
    }

    Vector3 RigidBody::getVelocity() const
    {
        return velocity;
    }

    void RigidBody::setRotation(const Vector3& rotation)
    {
        RigidBody::rotation = rotation;
    }

    Vector3 RigidBody::getRotation() const
    {
        return rotation;
    }
    
    Vector3 RigidBody::getForceAccum() const {
        return forceAccum;
    }
}