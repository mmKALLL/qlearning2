#include "Controller.hpp"
#include <vector>

Controller::Controller() {
	// TODO: Esa: Does the constructor need any functionality?
	// No need for gravity in top down physics
	b2Vec2 gravity(0.0f, 0.0f);
	world = new b2World(gravity);
}

const Track Controller::getTrack() const {
	return currentTrack;
}
const Car Controller::getCar() const {
	return currentCar;
}

const std::vector<float> Controller::getSightVector(int amount, int degrees) {
	// TODO: Olli/Jussi: Distances to walls in various directions, spanning a vision width of specified degrees.
	// As of 4.12 only works for uneven amount of rays and minimum amount is 3
	//Done'd
	return currentCar.getDistances(amount, degrees);
}

const std::vector<float> Controller::getCarPosition() const {
	// x and y-coordinates in that order
	return currentCar.getPosition();
}

float Controller::getCarDistanceFromMiddle() const {
	// TODO: Jussi: Implement function to get car middle point's distance from center of track
	return 0.0f;
}

float Controller::getCarVelocity() const {
	// TODO: Olli/Jussi: Return the car's current speed.
	//Done'd
	return currentCar.getVelocity();
}

float Controller::getCarDistanceTraveled() const {
	// TODO Olli/Jussi Return how far the car has gone on the track
	//				   ie how many checkpoints it has reached
	//Done'd, though it returns the amount of checkpoints passed
	return currentCar.getCheckpoints();
}

// Ask neural network to provide a set of actions.
std::vector<float> Controller::getCarAction(NeuralNetwork& nn) {
	std::vector<float> params = getSightVector(numberOfVisionLines, fieldOfView); // TODO: Use some external constants?
	params.push_back(getCarVelocity());
	return currentCar.getNetwork().getOutputValuesFromInputs(params);
}

float Controller::getFitness() const {
	// TODO Jussi/Esa/Simo
	return getCarVelocity();
}

float Controller::getFitness(double time) const {
	// TODO Jussi/Esa/Simo
	return getCarDistanceTraveled() / time;
}

//Ask physics where the car would end up with actions in param
std::vector<float> Controller::simulateStepForward(Car& car, float steer, float accelerate) const {
	// TODO: Return vector such that:
	// vector[0] true if hit by a wall, otherwise false
	// vector[1] x coordinate
	// vector[2] x coordinate
	// vector[3] velocity
	// vector[4] angle in degrees

	std::vector<float> result;
	result.push_back(currentCar.getCollisionStatus());
	result.insert(result.end(), currentCar.getPosition().begin(), currentCar.getPosition().end());
	result.push_back(currentCar.getVelocity());
	result.push_back(currentCar.getAngle());

	return result;
}

void Controller::stepForward() {
	// TODO: Esa: implement the main logic

	//Advances the physics simulation by one step
	world->Step(timeStep, velocityIterations, positionIterations);
	//According to the manual forces should be cleared after taking a step
	world->ClearForces();
	// Call this to have car update its collision status and do all steering related stuff
	//currentCar.update(float speed, float angle, int amount, int degrees);
}