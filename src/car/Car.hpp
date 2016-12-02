#ifndef QLEARNING_CAR_H
#define QLEARNING_CAR_H
#include <Box2D/Box2D.h>
#include "Physics.hpp"
#include <vector>
#include "../network/NeuralNetwork.hpp" // TODO: Shouldn't this go through the Controller?

class Car
{
public:
	// Constructors
	Car();
	~Car();

	// Function definitions
	void accelerate(int direction);
	void turn(int direction);
	std::vector<float> getPosition() const;
	float getVelocity() const;
	b2Body* getCarBody() const;
	b2Vec2 getForwardVelocity() const;
	b2Vec2 getLateralVelocity() const;
	
	std::vector<float> updateRays();
	std::vector<float> getDistances() const;
	
	// Variables for desired speed, current speed, current force applied and maximun force that can be applied
	float desiredSpeed = 0;
	float currentSpeed = 0;
	float force = 0;
	
	NeuralNetwork& getNetwork();

private:
	// Variables for the body amd maximum speeds
	b2Body* carBody = nullptr;
	float maxSpeed = 100;
	float maxReverse = 50;
	float maxDriveForce = 500;
	float maxReverseForce = 250;
	float MaxTurningForce = 500;
	float maxLateralImpulse = 7.5;
	std::vector<float> distances;
	
	b2World* world;
	Physics physics;

	NeuralNetwork network;
};



#endif
