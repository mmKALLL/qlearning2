#include "Car.hpp"


Car::Car(b2World* world) : world(world)
{
	physics = new Physics(world, this);
	b2BodyDef carBodyDef;
	carBodyDef.type = b2_dynamicBody;

	carBodyDef.position.Set(0, 0);
	carBodyDef.angle = 0;
	carBody = world->CreateBody(&carBodyDef);

	b2PolygonShape carShape;
	carShape.SetAsBox(20.0f, 15.0f);

	b2FixtureDef carFixtureDef;
	carFixtureDef.shape = &carShape;
	carFixtureDef.density = 0.01f;
	carFixtureDef.restitution = 0.5f;

	carBody = world->CreateBody(&carBodyDef);
	carBody->CreateFixture(&carFixtureDef);
	carBody->SetUserData(this);	
}

Car::~Car()
{
	world->DestroyBody(carBody);
	delete physics;
}

void Car::update(float speed, float angle)
{
	physics->updateFriction(carBody);
	accelerate(speed);
	turn(angle);
}

void Car::accelerate(float speed)
{
	float desiredSpeed = speed*maxSpeed;

	b2Vec2 currentForwardNormal = carBody->GetWorldVector(b2Vec2(1, 0));
	float currentSpeed = b2Dot(physics->getForwardVelocity(carBody), currentForwardNormal);

	if (desiredSpeed > currentSpeed && speed > 0) {
		carBody->ApplyForce(maxDriveForce * currentForwardNormal, carBody->GetWorldCenter(), true);
	}
	else if (desiredSpeed < currentSpeed && speed < 0) {
		carBody->ApplyForce(-maxReverseForce * currentForwardNormal, carBody->GetWorldCenter(), true);
	}
}


void Car::turn(float angle)
{
	carBody->ApplyTorque(angle*MaxTurningForce * 45 * getVelocity()/maxSpeed, true);
}

void Car::setParams(std::vector<float> position, float angle, float speed)
{
	b2Vec2 pos(position[0], position[1]);
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

std::vector<float> Car::getDistances(int amount, int degrees, std::vector<float> rayDistances) {
	distances = physics->updateRays(*carBody, amount, degrees, rayDistances);
	return distances;
}

int Car::getCheckpoints() const
{
	return checkpoints;
}

float Car::getMaxSpeed() const
{
	return maxSpeed;
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
	return{pos.x, pos.y};
}

float Car::getAngle() const
{
	return carBody->GetAngle() * RADTODEG;
}

float Car::getVelocity() const
{
	return b2Dot(physics->getForwardVelocity(carBody), carBody->GetWorldVector(b2Vec2(1, 0)));
}

void Car::setNetwork(NeuralNetwork newNetwork)
{
	network = newNetwork;
}

void Car::testDrive(){
	physics->updateFriction(carBody);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
    		this->update(1, 0);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
    		this->update(-1, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
    		this->update(0, -0.3);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
    		this->update(0, 0.3);
	}

	// For debugging
	std::cout << "Velocity: " << this->getVelocity() << std::endl;
	std::cout << "Angle: " << this->getAngle() << std::endl;
	std::cout << "Checkpoints: " << this->getCheckpoints() << std::endl;
	std::cout << "Collision: " << this->getCollisionStatus() << std::endl;
	std::vector<float> distances = getDistances(3, 180, {0.3f, 0.5f, 1.0f});
	std::cout << "Distance right: " << distances[0] << std::endl;
	std::cout << "Distance front: " << distances[1] << std::endl;
	std::cout << "Distance left: " << distances[2] << std::endl;
}
