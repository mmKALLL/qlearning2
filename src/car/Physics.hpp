#ifndef QLEARNING_PHYSICS_H
#define QLEARNING_PHYSICS_H
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#include <Box2D/Box2D.h>
#include <vector>

class Physics : public b2RayCastCallback
{
public:
	Physics(b2World* world);
	
	std::vector<float> updateRays(b2Body& carBody, int size, int degrees);
	void updateFriction(b2Body* carBody);
	void CarRayCallback();
	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
	
	b2Vec2 getForwardVelocity(b2Body* carBody) const;
	b2Vec2 getLateralVelocity(b2Body* carBody) const;

	bool m_hit;
	b2Vec2 m_point;
	b2Fixture* fixtureA;
	b2Vec2 normalA;

private:
	b2World* world;
	float maxLateralImpulse = 30;
	float rayLenght = 100;
};

#endif
