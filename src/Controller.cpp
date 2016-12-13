#include "Controller.hpp"

Controller::Controller() {
	initializeRun();
}

void Controller::initializeRun() {
	// No need for gravity in top down physics
	// currentTrack.setControllerReference(*this);
	m_world = new b2World(b2Vec2(0,0));
	currentCar = new Car(m_world);
	currentTrack = new Track(m_world, this);
	currentNetwork = NeuralNetwork(layerCount);
	currentCar->setNetwork(currentNetwork);
	stepCounter = 0;
}

const Car& Controller::getCar() const {
	return *currentCar;
}

const Track& Controller::getTrack() const {
	return *currentTrack;
}

const std::vector<float> Controller::getCarPosition() const {
	// x and y-coordinates in that order
	return currentCar->getPosition();
}

const std::vector<float> Controller::getSightVector(int amount, int degrees) {
	// TODO: Olli/Jussi: Distances to walls in various directions, spanning a vision width of specified degrees.
	// As of 4.12 only works for uneven amount of rays
	return currentCar->getDistances(amount, degrees);
}

float Controller::getCarDistanceFromMiddle() const {
	// TODO: Jussi: Implement function to get car middle point's distance from center of track
	return 0.0f;
}

float Controller::getCarVelocity() const {
	// Returns the current car's current speed.
	return currentCar->getVelocity();
}

float Controller::getCarDistanceTraveled() const {
	// TODO Olli/Jussi Return how far the car has gone on the track
	//				   ie how many checkpoints it has reached
	//Done'd, though it returns the amount of checkpoints passed
	return currentCar->getCheckpoints();
}

// Ask neural network to provide a set of actions.
std::vector<float> Controller::getCarAction(NeuralNetwork& nn) {
	std::vector<float> params = getSightVector(numberOfVisionLines, fieldOfView); // TODO: Use some external constants?
	params.push_back(getCarVelocity());
	return currentCar->getNetwork().getOutputValuesFromInputs(params);
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
	// ***************** NOT NEEDED *********************
	// TODO: Return vector such that:
	// vector[0] true if hit by a wall, otherwise false
	// vector[1] x coordinate
	// vector[2] x coordinate
	// vector[3] velocity
	// vector[4] angle in degrees

	std::vector<float> result;
	result.push_back(currentCar->getCollisionStatus());
	result.insert(result.end(), currentCar->getPosition().begin(), currentCar->getPosition().end());
	result.push_back(currentCar->getVelocity());
	result.push_back(currentCar->getAngle());

	return result;
}

void Controller::stepForward() {
	this->stepCounter += 1;
	explorationCoefficient > 0.05 ? explorationCoefficient -= 0.001 : true;
	
	// Get action from network, then make it learn.
	float prevVelocity = currentCar->getVelocity();
	std::vector<float> state = getSightVector(numberOfVisionLines, fieldOfView);
	state.push_back(prevVelocity);
	
	std::vector<float> action = currentNetwork.getAction(state, actionDepth, explorationCoefficient);
	currentCar->accelerate(action[0]);
	currentCar->turn(action[1]);
	
	float reward = currentCar->getCollisionStatus() * -10000 + currentCar->getVelocity() - prevVelocity * 0.9;
	
	float qtarget = qvalue + trainer.getStepSize() * (reward + discountFactor * action[2] - qvalue);
	trainer.adjustNetwork(currentNetwork, qvalue, qtarget);
	this->qvalue = qtarget;
	
	//Advances the physics simulation by one step
	m_world->Step(timeStep, velocityIterations, positionIterations);
	//According to the manual forces should be cleared after taking a step
	m_world->ClearForces();
	// Call this to have car update its collision status and do all steering related stuff
	//currentCar.update(float speed, float angle, int amount, int degrees);
}
