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
	const std::vector<float> getSightVector(int size, int degrees, std::vector<float> rayDistances);
	float getCarDistanceTraveled() const;
	float getCarRotation() const;
	float getCarVelocity() const;

	//---Controller actions
    void stepForward(float timeStep); //Moves simulation; makes a call to NeuralNetwork and asks Physics to parse action
    
private:
	
	/***** General settings *****/
	bool networkDebug;		// print network to console
	bool carDebug;		// manual driving
	int maxFastForwardRuns;	// Untested. Half-implemented. How many runs to do before terminating fastforward.
	bool writeActionsToFile;	// car driving history; overwrites existing history files
	
	int numberOfVisionLines;
	int fieldOfView;
	const std::vector<float> rayDistances{ 0.3f, 0.5f, 1.0f };
	
	/***** Simulation constants *****/
	float timeStep;				//the duration of one frame (60 FPS)
	int32 velocityIterations;	//how strongly to correct velocity
	int32 positionIterations;	//how strongly to correct position
	
	/***** Network building related constants *****/
	const std::vector<unsigned int> hiddenLayerSizes = std::vector<unsigned int> {6};		// Adjust network node topology with this.
	const unsigned int layerCount = 2 + hiddenLayerSizes.size(); 							// Don't touch.
	float nodeInitLow;						// Randomized initial node weights are between these
	float nodeInitHigh;
	int stateSize;		// Don't touch. Add current velocity into state input vector.
	
	/***** Action-space search and learning-related constants *****/
	int learningMode;							// Which weight adjustment algorithm to use. Supported modes: 0 (no learning), 1 (racist gradient descent).
	bool useSig;							// Whether to use sigmoid functions in network evaluation.
	float defaultStepSize; 				// Learning rate; multiplies learned outcome's impact on network node weights
	float actionDepth; 						// How many variations of acceleration/turning values to test. Primary performance impact in network eval. Up to ~80 should be manageable.
	float discountFactor; 					// [0.0f, 1.0f); importance of "later" vs "now", with higher values increasing the impact of "now"
	float explorationCoefficient = 5.0; 				// Weighs exploration over exploitation in Q-search; decreases on each step until minimum
	float minExplorationCoefficient;		// Don't touch.
	float explorationCoefficientDecrease;
	float prevWeightCoefficient;			// How large impact the previous weight's magnitude has in learning
	float prevValueCoefficient;				// How important the previous value of a node is. Closer to 1 means "keep it the same" and closer to  0 means "discard old value; make radical changes into the targets"
	float rewardMultiplier;					// Multiplier on reward values to prevent crashing from overflows.
	float qvalueMultiplier;					// Don't adjust unless the program crashes. Might make learning very buggy. Seek guidance from Esa and Simo first. You can not parse HTML with regex.
	
	/***** Reward function coefficients, see reward in Controller::takeStep() *****/
	float timeToFitnessMultiplier;			// Unused. Fitness function balancing multiplier.
	float wallPenalty;					// Reward penalty for hitting a wall.
	float prevVelocityCoefficient;			// Reward multiplier for increasing speed vs going fast. Higher value means that increasing car speed is good. Only [0.0f, 1.0f] are sensible.
	float velocityMultiplier;			// Multiplier for increasing float accuracy to reduce out of bounds exceptions.

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
