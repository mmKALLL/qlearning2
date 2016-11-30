#include "include\Box2D\Box2D.h"
#include <vector>

class Car
{

public:
	// Constructors
	Car(b2World * world, const b2Vec2& position, const b2Vec2& dimensions);
	~Car();

	// Function definitions
	void accelerate(int direction);
	void turn(int direction);
	b2Vec2 getForwardVelocity();
	b2Vec2 getLateralVelocity();
	void Car::updateFriction();
	std::vector<float> updateRays();
	std::vector<float> getDistances();
	
	
	
	// Variables for desired speed, current speed, current force applied and maximun force that can be applied
	float desiredSpeed = 0;
	float currentSpeed = 0;
	float force = 0;
	
	

private:
	// Variables for the body amd maximum speeds
	b2Body* carBody = nullptr;
	float maxSpeed = 100;
	float maxReverse = 50;
	float maxDriveForce = 500;
	float maxReverseForce = 250;
	float MaxTurningForce = 500;
	float maxLateralImpulse = 7.5;
	b2World* world = nullptr;
	std::vector<float> distances;

};

class Tire
{
public:
	Tire();
	~Tire();
};
