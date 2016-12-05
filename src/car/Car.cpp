#include "Car.hpp"


Car::Car(b2World* world)
{
	
	this->world = world;
	//Create definition for car body
	b2BodyDef carBodyDef;
	carBodyDef.type = b2_dynamicBody;

	//Set the initial position and angle and then create the body
	carBodyDef.position.Set(0, 0);
	carBodyDef.angle = 0; //set the starting angle
	carBody = world->CreateBody(&carBodyDef);
	carBody->SetAngularDamping(3);

	b2PolygonShape carShape;
	// Car is somewhat long and a bit wide
	carShape.SetAsBox(2.0f, 1.5f);

	b2FixtureDef carFixtureDef;
	carFixtureDef.shape = &carShape;
	// Density controls the mass of the car
	carFixtureDef.density = 1;
	// Restitution is how bouncy it is, eg. 0 no bounce and 1 all kinetic energy is conserved in collisions
	carFixtureDef.restitution = 0.25;
	// Well, friction
	carFixtureDef.friction = 0.5;
	carBody = world->CreateBody(&carBodyDef);
	carBody->CreateFixture(&carFixtureDef);
}


Car::~Car()
{
	carBody->GetWorld()->DestroyBody(carBody);
	
}



void Car::update(float speed, float angle, int amount, int degrees)
{
	physics.collisionCheck(carBody);
	physics.updateFriction(carBody);
	getDistances(amount, degrees);

	if (physics.collisionCheck(carBody) == 1) {
		checkpoints++;
	}
	else {
		collisionStatus = true;
	}
	accelerate(speed);
	turn(angle);
	
}

void Car::accelerate(float speed)
{
	// Get current forward speed and set force
	float force = 0;

	// Set desired speed in relation to if we are reversing or going forward
	desiredSpeed = speed*maxSpeed;


	b2Vec2 currentForwardNormal = carBody->GetWorldVector(b2Vec2(1, 0));

	currentSpeed = b2Dot(physics.getForwardVelocity(carBody), currentForwardNormal);

	//Depending on current speed the amount of force is determined
	if (desiredSpeed > currentSpeed) {
		force = maxDriveForce;
	}
	else if (desiredSpeed < currentSpeed) {
		force = -maxDriveForce;
	}
	else {
		force = 0;
	}

	// Apply the force
	carBody->ApplyForce(force * currentForwardNormal, carBody->GetWorldCenter(), false);
	
}


void Car::turn(float angle)
{
	carBody->ApplyTorque(angle*MaxTurningForce, true);
}


// function for returning just the distances to walls
std::vector<float> Car::getDistances(int amount, int degrees) {
	distances = physics.updateRays(*carBody, amount, degrees);
	return distances;
}

int Car::getCheckpoints() const
{
	return checkpoints;
}

bool Car::getCollisionStatus() const
{
	return collisionStatus;
}

NeuralNetwork & Car::getNetwork()
{
	return network;
}

std::vector<float> Car::getPosition() const
{
	b2Vec2 pos = carBody->GetPosition();
	std::vector<float> position;
	position.push_back(pos.x);
	position.push_back(pos.y);
	return position;
}

float Car::getAngle() const
{
	return carBody->GetAngle()*RADTODEG;
}

float Car::getVelocity() const
{
	return b2Dot(physics.getForwardVelocity(carBody), carBody->GetWorldVector(b2Vec2(1, 0)));
}

