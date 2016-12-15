#ifndef QLEARNING_CAR_H
#define QLEARNING_CAR_H
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
class Physics;
#include "Physics.hpp"
#include <vector>
#include "../network/NeuralNetwork.hpp" // TODO: Shouldn't this go through the Controller?

class Car
{

public:
	// Constructors
	Car(b2World*);
	~Car();

	// Getters
	float getAngle() const;
	float getVelocity() const;
	float getMaxSpeed() const;
	int getCheckpoints() const;
	bool getCollisionStatus() const;
	NeuralNetwork& getNetwork();
	std::vector<float> getPosition() const;
	std::vector<float> getDistances(int amount, int degrees);

	void setParams(std::vector<float> position, float angle, float speed);
	void setCollisionStatus(bool status);
	void setNetwork(NeuralNetwork newNetwork);

	void update(float speed, float angle);
	void accelerate(float speed);
	void turn(float angle);
	void addCheckpoint();
	void testDrive();
		
private:
	// Variables for the body amd maximum speeds
	// maxspeed is maximum speed and maxDriveForce is maximum acceleration
	b2Body* carBody = nullptr;
	float maxSpeed = 120;
	float maxDriveForce = 3000;
	float maxReverseForce = 1500;
	float MaxTurningForce = 25000;
	bool collisionStatus = false;
	std::vector<float> distances;
	int checkpoints = 0;
	
	
	b2World* world = nullptr;
	Physics* physics;
	NeuralNetwork network;

};

#endif
