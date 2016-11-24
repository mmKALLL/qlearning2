#include "Car.hpp"



Car::Car(b2World * world, const b2Vec2& position, const b2Vec2& dimensions)
{
	//Create definition for car body
	b2BodyDef carBodyDef;
	carBodyDef.type = b2_dynamicBody;

	//Set the initial position and angle and then create the body
	carBodyDef.position.Set(position.x, position.y);
	carBodyDef.angle = 0; //set the starting angle
	carBody = world->CreateBody(&carBodyDef);

	b2PolygonShape carShape;
	// Car is 4 meters long and 1.5 meter wide
	carShape.SetAsBox(4, 1.5);

	b2FixtureDef carFixtureDef;
	carFixtureDef.shape = &carShape;
	// Density controls the mass of the car
	carFixtureDef.density = 1;
	// Restitution is how bouncy it is, eg. 0 no bounce and 1 all kinetic energy is conserved
	carFixtureDef.restitution = 0.25;
	// Well, friction
	carFixtureDef.friction = 0.5;
}


Car::~Car()
{
	carBody->GetWorld()->DestroyBody(carBody);
	
}

void Car::accelerate()
{
	carBody->ApplyForce(b2Vec2(0, 50), carBody->GetWorldCenter(), true);
}

void Car::brake()
{
	carBody->ApplyForce(b2Vec2(0, 50), carBody->GetWorldCenter(), true);
}



Tire::Tire()
{
}


Tire::~Tire()
{
}
