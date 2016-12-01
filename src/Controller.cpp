#include "Controller.hpp"
#include <vector>

Controller::Controller() {
	// TODO: Esa: Implement constructor
	// TODO: Initialize GUI
}

const Track Controller::getTrack() const {
	return currentTrack; //this.currentTrack must have a class type
}
const Car Controller::getCar() const {
	return currentCar; //this.currentCar must have a class type
}

const std::vector<float> Controller::getSightVector(int amount, int degrees) const {
	// TODO: Olli/Jussi: Distances to walls in various directions, spanning a vision width of specified degrees.
}

float Controller::getCarDistanceFromMiddle() const {
	// TODO: Jussi: Implement function to get car middle point's distance from center of track
}

float Controller::getCarVelocity() const {
	// TODO: Olli/Jussi: Return the car's current speed.
}

const std::vector<float> Controller::getCarPosition() const {
	// TODO: Olli/Jussi: Return the car's current x/y position as two-element vector.
}

// Ask neural network to provide a set of actions.
const std::vector<double> Controller::getCarAction() {
	std::vector<double> params;
	params.push_back(getSightVector(numberOfVisionLines, fieldOfView)); // TODO: Use some external constants?
	params.push_back(getCarVelocity());
	return currentCar.network.getOutputsFromInputs(params);
}

double Controller::getFitness(double x, double y, double time) const {
	// TODO: Jussi/Esa/Simo get car's fitness based on xy and time
	// TODO Jussi: Do you need more parameters to determine
				// the distance a car has traveled on the track?
	// double dist = track.getDistanceTraveled( params );
}
double Controller::getFitness(Car& car, double time) const {
	// TODO Jussi/Esa/Simo
}

//Ask physics where the car would end up with actions in param
const std::vector<float> Controller::simulateStepForward(Car& car, float steer, float accelerate) const {
	// TODO: Return vector such that:
	// vector[0] vector[1] are the x,y coordinates
	// vector[2] is the velocity
	// vector[3] is the angle at which the car is facing maybe (???)
}

void Controller::stepForward() {
	// TODO: Esa: implement the main logic
}