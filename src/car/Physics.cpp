#include "Physics.hpp"

Physics::Physics()
{
	// No need for gravity in top down physics
	b2Vec2 gravity(0.0f, 0.0f);
	world = std::make_unique<b2World>(gravity);
}


Physics::~Physics()
{
	delete &world;
}
