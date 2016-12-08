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
	Car(b2World* world);
	~Car();

	// Getters
	float getAngle() const;
	float getVelocity() const;
	int getCheckpoints() const;
	bool getCollisionStatus() const;
	NeuralNetwork& getNetwork();
	std::vector<float> getPosition() const;
	std::vector<float> getDistances(int amount, int degrees);
	

	void update(float speed, float angle, int amount, int degrees); //Call this function to have car update its location speed etc
	void accelerate(float speed);
	void turn(float angle);
	void setParams(std::vector<float> position, float angle, float speed);
	
	// Variables for desired speed, current speed, current force applied and maximun force that can be applied
	float desiredSpeed = 0;
	float currentSpeed = 0;
	
	void setNetwork(NeuralNetwork newNetwork);

private:
	// Variables for the body amd maximum speeds
	// maxspeed is maximum speed and maxDriveForce is maximum acceleration
	b2Body* carBody = nullptr;
	float maxSpeed = 30;
	float maxDriveForce = 3000;
	float MaxTurningForce = 1000;
	std::vector<float> distances;
	int checkpoints = 0;
	bool collisionStatus = false;
	
	b2World* world;
	Physics physics = Physics(world);

	NeuralNetwork network;
};



#endif
