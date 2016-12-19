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
	std::vector<float> getDistances(int amount, int degrees, std::vector<float> rayDistances);
	// Setters
	void setParams(std::vector<float> position, float angle, float speed);
	void setCollisionStatus(bool status);
	void setNetwork(NeuralNetwork newNetwork);

	// Car's status updaters
	void update(float speed, float angle);
	void accelerate(float speed);
	void turn(float angle);
	void addCheckpoint();

	void testDrive();
		
private:
	b2Body* carBody = nullptr;
	float maxSpeed = 120; // Box2D effectively limits this to 120
	float driveForce = 2300;
	float brakeForce = 2300;
	float reverseForce = 727; // Caps the reverse speed at just under 40
	float turnRatio = 80000; // 80 000 seems to be the optimal value
	bool collisionStatus = false;
	std::vector<float> distances;
	int checkpoints = 0;

	b2World* world = nullptr;
	Physics* physics;
	NeuralNetwork network;
};
#endif
