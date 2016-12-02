#ifndef QLEARNING_PHYSICS_H
#define QLEARNING_PHYSICS_H
#include <Box2D/Box2D.h>
#include <vector>


class Physics
{

public:
	Physics();
	b2World* getWorld() const;
	std::vector<float> Physics::updateRays(b2Body& carBody);
	void Physics::updateFriction(b2Body* carBody);

	b2Vec2 Physics::getForwardVelocity(b2Body* carBody) const;
	b2Vec2 Physics::getLateralVelocity(b2Body* carBody) const;

private:
	b2World* world;

	// Simulation runs at 60 fps
	float32 timeStep = 1 / 60.0;
	int32 velocityIterations = 8;   //how strongly to correct velocity
	int32 positionIterations = 3;   //how strongly to correct position

	float maxLateralImpulse = 7.5; 
};

//myWorld->Step( timeStep, velocityIterations, positionIterations);
//Call the above function everytime you want the simulation to proceed, each call proceeds the simulation by 1/60 second.

#endif
