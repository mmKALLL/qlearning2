#ifndef QLEARNING_CAR_H
#define QLEARNING_CAR_H
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
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
	
	// Variables for desired speed, current speed, current force applied and maximun force that can be applied

	
	
private:
	// Variables for the body amd maximum speeds
	// maxspeed is maximum speed and maxDriveForce is maximum acceleration
	b2Body* carBody = nullptr;
	float maxSpeed = 250;
	float maxDriveForce = 3000;
	float maxReverseForce = 1000;
	float MaxTurningForce = 50000;
	std::vector<float> distances;
	int checkpoints = 0;
	bool collisionStatus = false;
	float desiredSpeed = 0;
	float currentSpeed = 0;
	
	b2World* world = nullptr;
	Physics physics = Physics(world);

	NeuralNetwork network;
	
};

class Collision : public b2ContactListener
{
	void BeginContact(b2Contact* contact) {

		//check if fixture A was a ball
		void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
		void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
		if (!userDataA) {
			if (contact->GetFixtureA()->IsSensor() == false) {
				static_cast<Car*>(userDataB)->setCollisionStatus(true);
			}
		}

		if (!userDataB) {
			if (contact->GetFixtureB()->IsSensor() == false) {
				static_cast<Car*>(userDataA)->setCollisionStatus(true);
			}
		}

	}

	void EndContact(b2Contact* contact) {

		void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
		void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
		if (!userDataA) {
			if (contact->GetFixtureA()->IsSensor() == true) {
				static_cast<Car*>(userDataB)->addCheckpoint();
			}
			else{
			static_cast<Car*>(userDataB)->setCollisionStatus(false);
			}
		}

		if (!userDataB) {
			if (contact->GetFixtureB()->IsSensor() == true) {
				static_cast<Car*>(userDataA)->addCheckpoint();
			}
			else{
			static_cast<Car*>(userDataA)->setCollisionStatus(false);
			}
		}
	}
};


#endif
