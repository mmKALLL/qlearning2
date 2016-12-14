#ifndef QLEARNING_PHYSICS_H
#define QLEARNING_PHYSICS_H
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#include <Box2D/Box2D.h>
#include <vector>



class Physics
{
public:
	Physics(b2World* world);
	

	std::vector<float> updateRays(b2Body& carBody, int size, int degrees);
	void updateFriction(b2Body* carBody);
	
	b2Vec2 getForwardVelocity(b2Body* carBody) const;
	b2Vec2 getLateralVelocity(b2Body* carBody) const;

private:
	b2World* world;
	float maxLateralImpulse = 7.5;
	
	
};



#endif
