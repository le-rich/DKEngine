#pragma once
#ifndef BODY_HEADER

#define BODY_HEADER

#include "core.h"

namespace AE86 {
	/** The basic simulation object in our engine. */
	class RigidBody {
	protected:
		/** 
		 * Holds the inverse mass of our rigid body. Integration
		 * becomes easier with an inverse mass, moreover, it is
		 * impossible to represent things that oughtn't be affected
		 * by forces in our engine like walls and ground, since they'd
		 * require infinite mass. Instead, we can represent it as a 0.0
		 * value inverseMass. Moreover, bodies with zero mass are point-
		 * less to keep track of.
		 */
		real inverseMass;

		/**
		 * Damping that is applied to linear motion on the body.
		 * It is required to remove energy over time and approximate
		 * "drag forces" on a body.
		 */
		real linearDamping;

		// World space coords for the rigid body.
		Vector3 position;

		// Angular orientation of the rigid body in world space */
		Quaternion orientation;

		// Linear velocity of the rigid body in world space.
		Vector3 velocity;

		// Holds angular velocity of the rigid body in world space.
		Vector3 rotation;

		/**
		 * Holds a transform matrix for converting body space into
		 * world space and vice versa. 
		 */
		Matrix4 transformMatrix;

		/**
		 * The inverse of the body's inertia tensor. 
		 * it cannot be a singular matrix. It is defined
		 * in body space.
		 */
		Matrix3 inverseInertiaTensor;

		/**
		 * The inverse inertia tensor of the body in world
		 * space.
s		 */
		Matrix3 inverseInertiaTensorWorld;

		/********************************************************
		 * The below two accumulators store the force and torque,
		 * respectively, to be applied on the rigid body during 
		 * the next integration step. It is an accumulation of all
		 * forces applied to it since the last integration step. 
		 * This is essentially D'Alembert's principle.
		 ********************************************************/

		// Accumulated force applied at next integration step.
		Vector3 forceAccum;

		// Accumulated torque applied at next integration step.
		Vector3 torqueAccum;


	public: 
		/** 
		 * The rigid body has internal data that is updated based on
		 * the values of its state. Whenever state changes, such as 
		 * angular orientation or position, things such as the 
		 * transformMatrix need to be recalculated. 
		 *
		 * 
		 * FUTURE NOTE: when we have the transform component, and its 
		 * location/orientation data is changed through game logic, 
		 * we must ensure that this method is explicitly called to ensure 
		 * that the rigid body simulation remains congruent to it.
		 */
		void calculateDerivedData();

		/**
		 * Sets the intertia tensor for the rigid body.
		 * This must be a full rank matrix and must be
		 * invertible.
		 */
		void setInertiaTensor(const Matrix3& inertiaTensor);

		// Converts given world space point into body space point
		Vector3 getPointInLocalSpace(const Vector3& point) const;

		// Converts given body space point into world space point
		Vector3 getPointInWorldSpace(const Vector3& point) const;

		/**
		 * Applies the given force on the center of mass of
		 * the rigid body. Force is in world coordinates.
		 */
		void addForce(const Vector3& force);

		/**
		 * Applies a force at a point on the rigid body.
		 * Both force and point are in world space coords.
		 * NOTE: this can result in angular acceleration as
		 * torque is calculated in this function call.
		 * 
		 * See generic addForce method to apply a force at the
		 * centre of mass and therefore incur no angular
		 * acceleration.
		 */
		void addForceAtPoint(const Vector3& force,
			const Vector3& point);

		/** 
		 * Applies a force at a point on the rigid body.
		 * Both force and point are in local space coords.
		 * 
		 * NOTE: this can result in angular acceleration as
		 * torque is calculated in this function call.
		 * 
		 * See generic addForce method to apply a force at the 
		 * centre of mass and therefore incur no angular 
		 * acceleration.
		 */
		void addForceAtBodyPoint(const Vector3& force,
			const Vector3& point);

		/** 
		 * Clears the forces and torques in the accumulators.
		 * Automatically applied at the end of each integration
		 * step.
		 */
		void clearAccumulators();

		/** 
		 * Integrates linear and angular position from
		 * currently-applied forces/torques.
		 */
		void integrate(real duration);
	};
	
}

#endif