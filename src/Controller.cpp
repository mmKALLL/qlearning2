#include "Controller.hpp"

Controller::Controller() {
	if (writeActionsToFile) {
		carActionFile.open("carActionError.txt");
	}
	runCounter = 0;
	initializeController();
}

Controller::~Controller() {
	for (auto layer : currentNetwork.nodes) {
		for (auto *node : layer) {
			std::cout << "Node " << node->toString() << " weights: " << std::endl;
			for (auto w : node->getConnectionsIn()) {
				std::cout << "    w" << (*std::get<0>(w)).toString() << " " << std::get<1>(w) << std::endl;
			}
		}
	}
	
}

void Controller::initializeController() {
	
	// No need for gravity in top down physics
	m_world = new b2World(b2Vec2(0,0));
	currentCar = new Car(m_world);
	
	currentNetwork = NeuralNetwork(layerCount);
	
	// Build network
	std::vector<unsigned int> layerSizes;
	layerSizes.push_back(stateSize + currentNetwork.extraInputs); // input layer size
	for (auto x : hiddenLayerSizes) {
		layerSizes.push_back(x);
	}
	layerSizes.push_back(1); // output layer
	
	currentNetwork.build(layerSizes, true, nodeInitLow, nodeInitHigh);
	
	currentTrack = new Track(m_world, this);
	initializeRun();
}

void Controller::initializeRun() {
	delete currentCar;
	currentCar = new Car(m_world);
	currentCar->setNetwork(currentNetwork);
	
	trainer = new Learning(defaultStepSize);
	stepCounter = 0;
	qvalue = 0.0f;
	runCounter += 1;
	
	if (writeActionsToFile) {
		carActionFile.close();
		std::string fileName = std::string("car") + std::to_string(runCounter) + std::string("_actions.txt");
		carActionFile.open(fileName.c_str(), std::ofstream::out | std::ofstream::trunc); // overwrite existing
	}
	std::cout << "Hello World! My name is initializeRun()-chan!" << std::endl;
	
	if (fastforward && runCounter < maxFastForwardRuns) {
		int prevrun = runCounter;
		while (prevrun == runCounter) {
			stepForward();
		}
	}
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
	// TODO: Function not required anymore?
	return 0.0f;
}

float Controller::getCarDistanceTraveled() const {
	// TODO Olli/Jussi Return how far the car has gone on the track
	//				   ie how many checkpoints it has reached
	//Done'd, though it returns the amount of checkpoints passed
	return currentCar->getCheckpoints();
}

float Controller::getCarRotation() const{
	return currentCar->getAngle();
}

float Controller::getCarVelocity() const {
	// Returns the current car's current speed.
	return currentCar->getVelocity();
}

/***** OLD CODE, USE STEPFORWARD() INSTEAD *****/
// Ask neural network to provide a set of actions.
/*std::vector<float> Controller::getCarAction(NeuralNetwork& nn) {
	std::vector<float> params = getSightVector(numberOfVisionLines, fieldOfView); // TODO: Use some external constants?
	params.push_back(getCarVelocity());
	return currentCar->getNetwork().getOutputValuesFromInputs(params);
}*/

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
	
	// Get action from network, then make it learn.
	if (explorationCoefficient > minExplorationCoefficient) {
		 explorationCoefficient -= explorationCoefficientDecrease;
	}
	float prevVelocity = currentCar->getVelocity();
	std::vector<float> state = getSightVector(numberOfVisionLines, fieldOfView);
	state.push_back(prevVelocity);
	
	std::vector<float> action = currentNetwork.getAction(state, actionDepth, explorationCoefficient, useSig);
	currentCar->update(action[0], action[1]);
	
	float reward = (currentCar->getCollisionStatus() * wallPenalty + (currentCar->getVelocity() * velocityMultiplier) - (prevVelocity * velocityMultiplier) * prevVelocityCoefficient) * rewardMultiplier;
	
	float qtarget = (qvalue + trainer->getStepSize() * (reward + discountFactor * action[2] - qvalue)) * qvalueMultiplier;
	trainer->adjustNetwork(*this, currentNetwork, qvalue, qtarget, learningMode);
	this->qvalue = qtarget;
	
	if (writeActionsToFile) {
		carActionFile << stepCounter << "," << action[0] << "," << action[1] << "," << action[2] << std::endl;
	}
	
	//currentCar->testDrive();
	if (currentCar->getCollisionStatus() == 1) {
		initializeRun();
	}

	//Advances the physics simulation by one step
	m_world->Step(timeStep, velocityIterations, positionIterations);
	//According to the manual forces should be cleared after taking a step
	m_world->ClearForces();

}
