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
	//Done'd
	return currentCar.getDistances();
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
	// vector[0] is 1 if the car has hit a wall, 0 otherwise
	// vector[1] vector[2] are the x,y coordinates ???
	// vector[3] is the velocity
	// vector[4] is the angle at which the car is facing maybe (???)
	std::vector<float> asd;
	return asd;
}

void Controller::stepForward() {
	// TODO: Esa: implement the main logic

	//Advances the physics simulation by one step
	world->Step(timeStep, velocityIterations, positionIterations);
}