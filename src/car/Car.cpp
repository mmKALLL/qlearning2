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
	carFixtureDef.density = 0.2f;

	carBody = world->CreateBody(&carBodyDef);
	carBody->CreateFixture(&carFixtureDef);
	carBody->SetUserData(this);
}

Car::~Car()
{
	world->DestroyBody(carBody);
	delete physics;
}

void Car::update(float speed, float direction)
{
	physics->updateFriction(carBody);
	accelerate(speed);
	turn(direction);
}

// Function takes parameter speed, which should be -1, 0 or 1. Positive speed prompts acceleration, while negative speed brakes or reverses, depending on the current velocity.
void Car::accelerate(float speed)
{
	if (speed > 0) {
		carBody->ApplyForce(driveForce * carBody->GetWorldVector(b2Vec2(1, 0)), carBody->GetWorldCenter(), true);
	}
	else if (speed < 0) {
		if (getVelocity() > 0) {
			carBody->ApplyForce(-brakeForce * carBody->GetWorldVector(b2Vec2(1, 0)), carBody->GetWorldCenter(), true);
		}
		else {
			carBody->ApplyForce(-reverseForce * carBody->GetWorldVector(b2Vec2(1, 0)), carBody->GetWorldCenter(), true);
		}
	}
}

// Function takes parameter direction, which should be in range [-1, 1]. Positive direction steers the car to the right, while negative direction steers the car to the left. When driving in reverse, it's the opposite.
void Car::turn(float direction)
{
	float currentSpeed = getVelocity();
	if (direction != 0) {
		if (currentSpeed > 1) {
			carBody->ApplyTorque(direction * turnRatio, true);
		}
		else if (currentSpeed < -1) {
			carBody->ApplyTorque(-direction * turnRatio, true);
		}
	}
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

std::vector<float> Car::getDistances(int amount, int degrees, std::vector<float> rayDistances)
{
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

void Car::testDrive()
{
	float acceleration = 0;
	float steering = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
    	acceleration++;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
    	acceleration--;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
    	steering--;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
    	steering++;
	}
	this->update(acceleration, steering);

	// Debug prints for driving
	// std::cout << "Velocity: " << this->getVelocity() << std::endl;
	// std::cout << "Collision: " << this->getCollisionStatus() << std::endl;
	// std::cout << "Angular velocity: " << carBody->GetAngularVelocity() << std::endl;
	// std::cout << "Angle: " << this->getAngle() << std::endl;
	// std::cout << "Checkpoints: " << this->getCheckpoints() << std::endl;
	// std::cout << "Collision: " << this->getCollisionStatus() << std::endl;
	// std::vector<float> distances = getDistances(3, 180, {0.3f, 0.5f, 1.0f});
	// std::cout << "Distance right: " << distances[0] << std::endl;
	// std::cout << "Distance front: " << distances[1] << std::endl;
	// std::cout << "Distance left: " << distances[2] << std::endl;
}
