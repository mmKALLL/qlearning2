#include "Controller.hpp"
#include <vector>

Controller::Controller() {
	// TODO: Esa: Does the constructor need any functionality?
}

const Track Controller::getTrack() const {
	return currentTrack; //this.currentTrack must have a class type
}
const Car Controller::getCar() const {
	return currentCar; //this.currentCar must have a class type
}

const std::vector<float> Controller::getSightVector(Car& car, int amount, int degrees) const {
	// TODO: Olli/Jussi: Distances to walls in various directions, spanning a vision width of specified degrees.
}

const std::vector<float> Controller::getCarPosition() const {
	// TODO: Olli/Jussi: Return the car's current x/y position as two-element vector.
}

float Controller::getCarDistanceFromMiddle(Car& car) const {
	// TODO: Jussi: Implement function to get car middle point's distance from center of track
}

float Controller::getCarVelocity(Car& car) const {
	// TODO: Olli/Jussi: Return the car's current speed.
}

float Controller::getCarDistanceTraveled(Car& car) const {
	// TODO Olli/Jussi Return how far the car has gone on the track
	//				   ie how many checkpoints it has reached
}

// Ask neural network to provide a set of actions.
std::vector<double> Controller::getCarAction(Car& car, NeuralNetwork& nn) {
	std::vector<float> params = getSightVector(car, numberOfVisionLines, fieldOfView); // TODO: Use some external constants?
	params.push_back(getCarVelocity(car));
	return currentCar.getNetwork().getOutputValuesFromInputs(params);
}

double Controller::getFitness(Car& car) const {
	// TODO Jussi/Esa/Simo
	return getCarVelocity(car);
}

double Controller::getFitness(Car& car, double time) const {
	// TODO Jussi/Esa/Simo
	return getCarDistanceTraveled(car) / time;
}

//Ask physics where the car would end up with actions in param
std::vector<float> Controller::simulateStepForward(Car& car, float steer, float accelerate) const {
	// TODO: Return vector such that:
	// vector[0] vector[1] are the x,y coordinates
	// vector[2] is the velocity
	// vector[3] is the angle at which the car is facing maybe (???)
}

void Controller::stepForward() {
	// TODO: Esa: implement the main logic
}