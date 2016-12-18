#include "Controller.hpp"

Controller::Controller() {
	reader = new FileReader();

	std::cout << "Loading constants...";

	/***** General settings *****/
	networkDebug = &(reader->parsedBool.at("networkDebug"));		// print network to console
	carDebug = reader->parsedBool.at("carDebug");		// manual driving
	fastforward = reader->parsedBool.at("fastforward");		// disable GUI
	maxFastForwardRuns = reader->parsedInt.at("maxFastForwardRuns");	// Untested. Half-implemented. How many runs to do before terminating fastforward.

	writeActionsToFile = reader->parsedBool.at("networkDebug");	// car driving history; overwrites existing history files
	numberOfVisionLines = reader->parsedInt.at("numberOfVisionLines");
	fieldOfView = reader->parsedInt.at("fieldOfView"); // TODO: FoV slider

						   /***** Simulation constants *****/
	 velocityIterations = reader->parsedInt.at("velocityIterations");   //how strongly to correct velocity
	 positionIterations = reader->parsedInt.at("positionIterations");   //how strongly to correct position

									  /***** Network building related constants *****/
	 nodeInitLow = reader->parsedFloat.at("nodeInitLow");						// Randomized initial node weights are between these
	 nodeInitHigh = reader->parsedFloat.at("nodeInitHigh");

	/***** Action-space search and learning-related constants *****/
	 learningMode = reader->parsedInt.at("learningMode");							// Which weight adjustment algorithm to use. Supported modes: 0 (no learning), 1 (racist gradient descent).
	 useSig = reader->parsedBool.at("useSig");							// Whether to use sigmoid functions in network evaluation.
	 defaultStepSize = reader->parsedFloat.at("defaultStepSize"); 				// Learning rate; multiplies learned outcome's impact on network node weights
	 actionDepth = reader->parsedFloat.at("actionDepth"); 						// How many variations of acceleration/turning values to test. Primary performance impact in network eval. Up to ~80 should be manageable.
	 discountFactor = reader->parsedFloat.at("discountFactor"); 					// [0.0f, 1.0f); importance of "later" vs "now", with higher values increasing the impact of "now"

	  minExplorationCoefficient = reader->parsedFloat.at("minExplorationCoefficient");		// Don't touch.
	  explorationCoefficientDecrease = reader->parsedFloat.at("explorationCoefficientDecrease");
	  prevWeightCoefficient = reader->parsedFloat.at("prevWeightCoefficient");			// How large impact the previous weight's magnitude has in learning
	  prevValueCoefficient = reader->parsedFloat.at("prevValueCoefficient");				// How important the previous value of a node is. Closer to 1 means "keep it the same" and closer to  0 means "discard old value; make radical changes into the targets"
	  rewardMultiplier = reader->parsedFloat.at("rewardMultiplier");					// Multiplier on reward values to prevent crashing from overflows.
	  qvalueMultiplier = reader->parsedFloat.at("qvalueMultiplier");					// Don't adjust unless the program crashes. Might make learning very buggy. Seek guidance from Esa and Simo first. You can not parse HTML with regex.

													/***** Reward function coefficients, see reward in Controller::takeStep() *****/
	  timeToFitnessMultiplier = reader->parsedFloat.at("timeToFitnessMultiplier");			// Unused. Fitness function balancing multiplier.
	  wallPenalty = reader->parsedFloat.at("wallPenalty");					// Reward penalty for hitting a wall.
	  prevVelocityCoefficient = reader->parsedFloat.at("prevVelocityCoefficient");			// Reward multiplier for increasing speed vs going fast. Higher value means that increasing car speed is good. Only [0.0f, 1.0f] are sensible.
	  velocityMultiplier = reader->parsedFloat.at("velocityMultiplier");			// Multiplier for increasing float accuracy to reduce out of bounds exceptions.

	  stateSize = numberOfVisionLines + 1;

	  std::cout << " Done!" << endl;

	if (writeActionsToFile) {
		carActionFile.open("carActionError.txt");
	}
	runCounter = 0;
	initializeController();
}

Controller::~Controller() {
	
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

	//
	std::cout << "numberOfVisionLines : " << numberOfVisionLines << endl;
	std::cout << "numberOfVisionLines + 1: " << (numberOfVisionLines +1) << endl;
	std::cout << "..which should equal to statesize " << endl;
	std::cout << "Statesize : " << stateSize << endl;
	std::cout << "Statesize + 3: " << (stateSize + 3) << endl;

	for (int s : layerSizes) {
		std::cout << s << endl;
	}
	std::cout << " " << nodeInitLow << " " << nodeInitHigh << endl;
	//

	currentNetwork.build(layerSizes, true, nodeInitLow, nodeInitHigh);

	initializeRun();
	currentTrack = new Track(m_world, this);

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
			stepForward(1.0f / 60.0f);
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
	// As of 4.12 only works for uneven amount of rays
	return currentCar->getDistances(amount, degrees);
}

float Controller::getCarDistanceTraveled() const {
	return currentCar->getCheckpoints();
}

float Controller::getCarRotation() const{
	return currentCar->getAngle();
}

float Controller::getCarVelocity() const {
	// Returns the current car's current speed.
	return currentCar->getVelocity();
}

void Controller::stepForward(float timeStep) {
	stepCounter += 1;
	
	// Print network
	if (networkDebug) {
		std::cout << std::endl << std::endl << "** FRAME " << stepCounter << " **" << std::endl << "-------------" << std::endl << std::endl;
		for (auto layer : currentNetwork.nodes) {
			for (auto *node : layer) {
				std::cout << "Node " << node->toString() << " weights: " << std::endl;
				for (auto w : node->getConnectionsIn()) {
					std::cout << "    w" << (*std::get<0>(w)).toString() << ", Weight: " << std::get<1>(w) << std::endl;
				}
			}
		}
	}
	
	if (!carDebug) {
		// Get action from network, then make it learn.
		if (explorationCoefficient > minExplorationCoefficient) {
		 	explorationCoefficient -= explorationCoefficientDecrease;
		}
		float prevVelocity = currentCar->getVelocity();
		std::vector<float> sights = getSightVector(numberOfVisionLines, fieldOfView);
		std::vector<float> state;
		for (float x : sights) {
			state.push_back(x / 400.0f);
		}
		state.push_back(prevVelocity * velocityMultiplier);
		
		std::vector<float> action = currentNetwork.getAction(state, actionDepth, 	explorationCoefficient, useSig);
		currentCar->update(action[0], action[1]);
		
		float reward = (currentCar->getCollisionStatus() * wallPenalty + 	(currentCar->getVelocity() * velocityMultiplier) - (prevVelocity * 	velocityMultiplier) * prevVelocityCoefficient) * rewardMultiplier;
		std::cout << "       reward is: " << reward << std::endl;
		std::cout << "       qvalue is: " << qvalue << std::endl;
		std::cout << "       action[2] is: " << action[2] << std::endl;
		std::cout << "       getStepSize is: " << /*trainer->getStepSize()*/defaultStepSize << std::endl;
		std::cout << "       discountFactor is: " << discountFactor << std::endl;
		std::cout << "       qvalueMultiplier is: " << qvalueMultiplier << std::endl;
		
		
		float qtarget = (qvalue + /*TODO: trainer->getStepSize()*/defaultStepSize * (reward + discountFactor * 	action[2] - qvalue)) * qvalueMultiplier;
		trainer->adjustNetwork(*this, currentNetwork, qvalue, qtarget, learningMode);
		this->qvalue = qtarget;
		
		if (writeActionsToFile) {
			carActionFile << stepCounter << "," << action[0] << "," << action[1] << "," << 	action[2] << std::endl;
		}
		
		//currentCar->testDrive();
		if (currentCar->getCollisionStatus() == 1) {
			initializeRun();
		}
	} else {
		currentCar->testDrive();
	}

	//Advances the physics simulation by one step
	m_world->Step(timeStep, velocityIterations, positionIterations);
	//According to the manual forces should be cleared after taking a step
	m_world->ClearForces();

}
