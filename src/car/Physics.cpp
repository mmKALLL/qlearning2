#include "Physics.hpp"

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
