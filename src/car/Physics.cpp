#include "Physics.hpp"

class CarRayCallback : public b2RayCastCallback
{
public:
	CarRayCallback()
	{
		m_hit = false;
	}

	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
	{
		m_hit = true;
		m_point = point;
		return fraction;
	}

	bool m_hit;
	b2Vec2 m_point;
};




Physics::Physics()
{
	// No need for gravity in top down physics
	b2Vec2 gravity(0.0f, 0.0f);
	world = new b2World(gravity);
}

b2World* Physics::getWorld() const
{
	return world;
}


std::vector<float> Physics::updateRays(b2Body& carBody) {

	std::vector<float> distances;
	// Rays for checking the distance
	// Initialized with starting and ending points
	b2Vec2 forwardRayStart = carBody.GetWorldPoint(b2Vec2(2, 0));
	b2Vec2 forwardRayEnd = carBody.GetWorldPoint(b2Vec2(10, 0));

	b2Vec2 leftRayStart = carBody.GetWorldPoint(b2Vec2(2, 0.75));
	b2Vec2 leftRayEnd = carBody.GetWorldPoint(b2Vec2(6, 6));

	b2Vec2 rightRayStart = carBody.GetWorldPoint(b2Vec2(2, -0.75));
	b2Vec2 rightRayEnd = carBody.GetWorldPoint(b2Vec2(6, -6));

	// Callbacks for checking if ray collides with something
	CarRayCallback callback;
	CarRayCallback callback1;
	CarRayCallback callback2;

	// Add rays to the world and if collision happens check for distance
	world->RayCast(&callback, forwardRayStart, forwardRayEnd);
	if (callback.m_hit) {
		distances.push_back((forwardRayStart - callback.m_point).Length());
	}
	else {
		distances.push_back(10);
	}

	world->RayCast(&callback1, leftRayStart, leftRayEnd);
	if (callback1.m_hit) {
		distances.push_back((leftRayStart - callback1.m_point).Length());
	}
	else {
		distances.push_back(10);
	}

	world->RayCast(&callback2, rightRayStart, rightRayEnd);
	if (callback2.m_hit) {
		distances.push_back((rightRayStart - callback2.m_point).Length());
	}
	else {
		distances.push_back(10);
	}


	return distances;
}