
#include "../include/FGen.h"

using namespace AE86;


void Gravity::updateForce(RigidBody* body, real duration) {
	// infinite mass means we do not move
	if (!body->hasFiniteMass()) return;

	body->addForce(gravity * body->getMass());
}

Gravity::Gravity(Vector3& gravityAccel) : gravity(gravityAccel) {}


void Spring::updateForce(RigidBody* body, real duration) {
	
	// world space cordinates of both ends of hte spring
	Vector3 lws = body->getPointInWorldSpace(connectionPoint);
	Vector3 ows = other->getPointInWorldSpace(otherConnectionPoint);

	Vector3 force = lws - ows; // spring force vector

	// magnitude of the force calcualted here
	real magnitude = real_abs(force.magnitude() - restLength) * springConstant;

	// the final force is applied
	force.normalize();
	force *= -magnitude;
	body->addForceAtPoint(force, lws);
}