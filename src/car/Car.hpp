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
	
	 //Call this function to have car update its location, speed, etc.
	void update(float speed, float angle);
	
	void accelerate(float speed);
	void turn(float angle);
	void setParams(std::vector<float> position, float angle, float speed);
	void setCollisionStatus();
	void addCheckpoint();
	void testDrive();
	
	// Variables for desired speed, current speed, current force applied and maximun force that can be applied
	float desiredSpeed = 0;
	float currentSpeed = 0;
	
	void setNetwork(NeuralNetwork newNetwork);

private:
	// Variables for the body amd maximum speeds
	// maxspeed is maximum speed and maxDriveForce is maximum acceleration
	b2Body* carBody = nullptr;
	float maxSpeed = 200;
	float maxDriveForce = 500000;
	float MaxTurningForce = 5000;
	std::vector<float> distances;
	int checkpoints = 0;
	bool collisionStatus = false;
	
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
				static_cast<Car*>(userDataB)->setCollisionStatus();
			}
		}

		if (!userDataB) {
			if (contact->GetFixtureB()->IsSensor() == false) {
				static_cast<Car*>(userDataA)->setCollisionStatus();
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
		}

		if (!userDataB) {
			if (contact->GetFixtureB()->IsSensor() == true) {
				static_cast<Car*>(userDataA)->addCheckpoint();
			}
		}
	}
};


#endif
