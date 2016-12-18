#ifndef QLEARNING_CONTROLLER_H
#define QLEARNING_CONTROLLER_H
#include <Box2D/Box2D.h>
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
#include "network/NeuralNetwork.hpp"
#include "car/Car.hpp"
class Learning;
#include "network/Learning.hpp"
class Track;
#include "car/Track.hpp"
#include "FileReader.hpp"

class Controller {
	friend class Learning;
public:
	Controller();
	~Controller();
	void initializeController(/*params?*/); // Application launched or previous car's run completely over, so start new run
	void initializeRun();
	
	//---Generic getters
	const Car& getCar() const;
    	const Track& getTrack() const;

	//std::vector<double>& getActionForCar(Car& car, NeuralNetwork& nn) const;

	//---Getters for car
	const std::vector<float> getCarPosition() const;
	const std::vector<float> getSightVector(int size, int degrees);
	float getCarDistanceTraveled() const;
	float getCarRotation() const;
	float getCarVelocity() const;

	//---Controller actions
    void stepForward(float timeStep); //Moves simulation; makes a call to NeuralNetwork and asks Physics to parse action
    
private:
	
	/***** General settings *****/
	const bool networkDebug;		// print network to console
	const bool carDebug;		// manual driving
	const bool fastforward;		// disable GUI
	const int maxFastForwardRuns;	// Untested. Half-implemented. How many runs to do before terminating fastforward.
	
	const bool writeActionsToFile;	// car driving history; overwrites existing history files
	const int numberOfVisionLines;
	const int fieldOfView; // TODO: FoV slider
	
	/***** Simulation constants *****/
	const int32 velocityIterations;   //how strongly to correct velocity
	const int32 positionIterations;   //how strongly to correct position
	
	/***** Network building related constants *****/
	const std::vector<unsigned int> hiddenLayerSizes = std::vector<unsigned int> {6};		// Adjust network node topology with this.
	const unsigned int layerCount = 2 + hiddenLayerSizes.size(); 							// Don't touch.
	const float nodeInitLow;						// Randomized initial node weights are between these
	const float nodeInitHigh;
	const int stateSize = numberOfVisionLines + 1;		// Don't touch. Add current velocity into state input vector.
	
	/***** Action-space search and learning-related constants *****/
	const int learningMode;							// Which weight adjustment algorithm to use. Supported modes: 0 (no learning), 1 (racist gradient descent).
	const bool useSig;							// Whether to use sigmoid functions in network evaluation.
	const float defaultStepSize; 				// Learning rate; multiplies learned outcome's impact on network node weights
	const float actionDepth; 						// How many variations of acceleration/turning values to test. Primary performance impact in network eval. Up to ~80 should be manageable.
	const float discountFactor; 					// [0.0f, 1.0f); importance of "later" vs "now", with higher values increasing the impact of "now"
	float explorationCoefficient = 5.0; 				// Weighs exploration over exploitation in Q-search; decreases on each step until minimum
	const float minExplorationCoefficient;		// Don't touch.
	const float explorationCoefficientDecrease;
	const float prevWeightCoefficient;			// How large impact the previous weight's magnitude has in learning
	const float prevValueCoefficient;				// How important the previous value of a node is. Closer to 1 means "keep it the same" and closer to  0 means "discard old value; make radical changes into the targets"
	const float rewardMultiplier;					// Multiplier on reward values to prevent crashing from overflows.
	const float qvalueMultiplier;					// Don't adjust unless the program crashes. Might make learning very buggy. Seek guidance from Esa and Simo first. You can not parse HTML with regex.
	
	/***** Reward function coefficients, see reward in Controller::takeStep() *****/
	const float timeToFitnessMultiplier;			// Unused. Fitness function balancing multiplier.
	const float wallPenalty;					// Reward penalty for hitting a wall.
	const float prevVelocityCoefficient;			// Reward multiplier for increasing speed vs going fast. Higher value means that increasing car speed is good. Only [0.0f, 1.0f] are sensible.
	const float velocityMultiplier;			// Multiplier for increasing float accuracy to reduce out of bounds exceptions.

	/***** Controller variables *****/
	b2World* m_world;
	Car* currentCar;
	Track* currentTrack;
	Learning* trainer;
	NeuralNetwork currentNetwork;
	std::ofstream carActionFile;
	int runCounter;
	int stepCounter;
	float qvalue;
	FileReader* reader;
};

#endif
