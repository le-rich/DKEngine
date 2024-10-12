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
	};
	
}

#endif
