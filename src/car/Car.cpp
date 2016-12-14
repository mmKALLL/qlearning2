#include "Car.hpp"
Collision collision;


Car::Car(b2World* world) : world(world)
{
	
	//Create definition for car body
	b2BodyDef carBodyDef;
	carBodyDef.type = b2_dynamicBody;


	//Set the initial position and angle and then create the body
	carBodyDef.position.Set(0, 0);
	carBodyDef.angle = 0; 
	carBody = world->CreateBody(&carBodyDef);

	b2PolygonShape carShape;
	// Car is somewhat long and a bit wide
	carShape.SetAsBox(2.0f, 1.5f);

	b2FixtureDef carFixtureDef;
	carFixtureDef.shape = &carShape;
	// Density controls the mass of the car
	carFixtureDef.density = 0.5;

	carBody = world->CreateBody(&carBodyDef);
	carBody->CreateFixture(&carFixtureDef);
	this->world->SetContactListener(&collision);
	carBody->SetUserData(this);

}

Car::~Car()
{
	
	
}

void Car::update(float speed, float angle)
{
	physics.updateFriction(carBody);
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
		carBody->ApplyForce(force * currentForwardNormal, carBody->GetWorldCenter(), false);
	}



	// Apply the force
	
	
}


void Car::turn(float angle)
{
	carBody->ApplyTorque(angle*MaxTurningForce, true);
}

void Car::setParams(std::vector<float> position, float angle, float speed)
{
	b2Vec2 pos(position[0], position[1]);
	// sin and cos might be otherway around
	b2Vec2 vel(speed*sin(angle*DEGTORAD), speed*cos(angle*DEGTORAD));
	carBody->SetLinearVelocity(vel);
	carBody->SetTransform(pos, angle*DEGTORAD);
}

void Car::setCollisionStatus()
{
	collisionStatus = true;
}

void Car::addCheckpoint()
{
	checkpoints += 1;
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
	return carBody->GetAngle() * RADTODEG;
}

float Car::getVelocity() const
{
	return b2Dot(physics.getForwardVelocity(carBody), carBody->GetWorldVector(b2Vec2(1, 0)));
}

void Car::setNetwork(NeuralNetwork newNetwork) {
	network = newNetwork;
}

void Car::testDrive(){
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right) )
	{
    		// left key is pressed: move our character
    		this->update(1, 0.3);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left) )
	{
    		// left key is pressed: move our character
    		this->update(1, -0.3);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
    		// left key is pressed: move our character
    		this->update(0, -0.3);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
    		// left key is pressed: move our character
    		this->update(0, 0.3);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
    		// left key is pressed: move our character
    		this->update(1, 0);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
    		// left key is pressed: move our character
    		this->update(-1, 0);
	}
	// FOr debugging
	std::cout << "Velocity: " << this->getVelocity() << std::endl;
	std::cout << "Angle: " << this->getAngle() << std::endl;
	std::cout << "Checkpoints: " << this->getCheckpoints() << std::endl;
	std::cout << "Collision: " << this->getCollisionStatus() << std::endl;
}
