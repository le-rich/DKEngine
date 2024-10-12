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

		// Holds angular velocit of the rigid body in world space.
		Vector3 rotation;

		/**
		 * Holds a transform matrix for converting body space into
		 * world space and vice versa. 
		 */
		Matrix4 transformMatrix;
	};
	
}

#endif
