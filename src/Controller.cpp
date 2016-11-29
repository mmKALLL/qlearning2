#include "Controller.hpp"
#include <vector>

Controller::Controller() {
	// TODO: Esa: Implement constructor
}

const std::vector<float> getSightVector(int size, int degrees) const {
	// TODO: Olli/Jussi: Distances to walls in various directions, spanning a vision width of specified degrees.
}

const float getCarDistanceFromMiddle() const {
	// TODO: Jussi: Implement function to get car middle point's distance from center of track
}

const float getCarSpeed() const {
	// TODO: Olli/Jussi: Return the car's current speed.
}

const vector<float> getCarPosition() const {
	// TODO: Olli/Jussi: Return the car's current x/y position as two-element vector.
}

void Controller::stepForward() {
	// TODO: Esa: implement the main logic
}

