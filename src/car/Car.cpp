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

	// Car is somewhat long and a bit wide
	b2PolygonShape carShape;
	carShape.SetAsBox(20.0f, 15.0f);

	b2FixtureDef carFixtureDef;
	carFixtureDef.shape = &carShape;
	carFixtureDef.density = 0.01f;
	carFixtureDef.restitution = 0.5f;

	carBody = world->CreateBody(&carBodyDef);
	carBody->CreateFixture(&carFixtureDef);
	carBody->SetUserData(this);

	this->world->SetContactListener(&collision);
	

}

Car::~Car()
{
	world->DestroyBody(carBody);
	
}

void Car::update(float speed, float angle)
{
	physics.updateFriction(carBody);
	accelerate(speed);
	turn(angle);
	

	
}

void Car::accelerate(float speed)
{

	// Set desired speed in relation to if we are reversing or going forward
	desiredSpeed = speed*maxSpeed;


	b2Vec2 currentForwardNormal = carBody->GetWorldVector(b2Vec2(1, 0));

	currentSpeed = b2Dot(physics.getForwardVelocity(carBody), currentForwardNormal);

	//Depending on current speed the amount of force is determined
	if (desiredSpeed > currentSpeed && speed > 0) {
		carBody->ApplyForce(maxDriveForce * currentForwardNormal, carBody->GetWorldCenter(), true);
	}
	else if (desiredSpeed < currentSpeed && speed < 0) {
		carBody->ApplyForce(-maxReverseForce * currentForwardNormal, carBody->GetWorldCenter(), true);
	}
}


void Car::turn(float angle)
{
	
	carBody->ApplyTorque(angle*MaxTurningForce * 45*  getVelocity()/maxSpeed, true);
}

void Car::setParams(std::vector<float> position, float angle, float speed)
{
	b2Vec2 pos(position[0], position[1]);
	// sin and cos might be otherway around
	b2Vec2 vel(speed*sin(angle*DEGTORAD), speed*cos(angle*DEGTORAD));
	carBody->SetLinearVelocity(vel);
	carBody->SetTransform(pos, angle*DEGTORAD);
}

void Car::setCollisionStatus(bool status)
{
	collisionStatus = status;
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

void Car::setNetwork(NeuralNetwork newNetwork)
{
	network = newNetwork;
}

void Car::testDrive(){
	physics.updateFriction(carBody);
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
    		// left key is pressed: move our character
    		this->update(-1, 0);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
    		// left key is pressed: move our character
    		this->update(1, 0);
	}

	// FOr debugging
	std::cout << "Velocity: " << this->getVelocity() << std::endl;
	std::cout << "Angle: " << this->getAngle() << std::endl;
	std::cout << "Checkpoints: " << this->getCheckpoints() << std::endl;
	std::cout << "Collision: " << this->getCollisionStatus() << std::endl;
}
