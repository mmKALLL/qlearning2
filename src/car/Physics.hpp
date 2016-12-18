#ifndef QLEARNING_PHYSICS_H
#define QLEARNING_PHYSICS_H
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#include <Box2D/Box2D.h>
#include <vector>
class Car;
#include "Car.hpp"

class Physics : public b2ContactListener, public b2RayCastCallback
{

public:
	Physics(b2World* world, Car* car);
	
	// Car's vision related functions
	std::vector<float> updateRays(b2Body& carBody, int size, int degrees, std::vector<float> rayDistances);
	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);

	// Friction
	void updateFriction(b2Body* carBody);
	b2Vec2 getForwardVelocity(b2Body* carBody) const;
	b2Vec2 getLateralVelocity(b2Body* carBody) const;

	// Collision checking
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	
private:
	b2World* world;
	Car* car;
	float maxLateralImpulse = 30;
	float rayLenght = 150;
	bool m_hit;
	b2Vec2 m_point;
	float32 m_fraction;
	b2Vec2 m_normal;
};

#endif
