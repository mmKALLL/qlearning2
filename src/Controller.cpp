#include "Controller.hpp"
#include <vector>

Controller::Controller() {
	// TODO: Esa: Implement constructor
}

const Track getTrack() const {
	return this.currentTrack;
}
const Car getCar() const {
	return this.currentCar;
}

const std::vector<float> getSightVector(int amount, int degrees) const {
	// TODO: Olli/Jussi: Distances to walls in various directions, spanning a vision width of specified degrees.
}

const float getCarDistanceFromMiddle() const {
	// TODO: Jussi: Implement function to get car middle point's distance from center of track
}

const float getCarSpeed() const {
	// TODO: Olli/Jussi: Return the car's current speed.
}

const std::vector<float> getCarPosition() const {
	// TODO: Olli/Jussi: Return the car's current x/y position as two-element vector.
}

void Controller::stepForward() {
	// TODO: Esa: implement the main logic
}

// Ask neural network to provide a set of actions.
const std::vector<double> getCarAction() {
	std::vector<double> params();
	params.push_back(getSightVector(5, 90)); // TODO: Use some external constants?
	params.push_back(getCarSpeed());
	return this.currentCar.network.getOutputsFromInputs(params)
}

//Ask physics where the car would end up with actions in param
const std::vector<float> Controller::simulateStepForward(Car& car, float steer, float accelerate) const {
	// TODO: Return vector such that:
		// vector[0] vector[1] are the x,y coordinates
		// vector[2] is the velocity
		// vector[3] is the angle at which the car is facing maybe (???)
}

const double getFitness(double x, double y, double time) const {
	// TODO: Jussi/Esa/Simo get car's fitness based on xy and time
	// TODO Jussi: Do you need more parameters to determine
				// the distance a car has traveled on the track?
	// double dist = track.getDistanceTraveled( params );
}
const double getFitness(Car& car, double time) const {
	// TODO Jussi/Esa/Simo
}