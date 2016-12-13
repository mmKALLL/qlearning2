#ifndef QLEARNING_CONTROLLER_H
#define QLEARNING_CONTROLLER_H
#include <Box2D/Box2D.h>
#include <vector>
#include <tuple>
#include "network/NeuralNetwork.hpp"
#include "network/Learning.hpp"
#include "car/Car.hpp"

class Track;
#include "car/Track.hpp"


class Controller {
public:
	Controller();
	void initializeRun(/*params?*/); // Application launched or previous car's run completely over, so start new run
	
	//---Generic getters
	const Car& getCar() const;
    const Track& getTrack() const;

	//std::vector<double>& getActionForCar(Car& car, NeuralNetwork& nn) const;

	//---Getters for car
	const std::vector<float> getCarPosition() const;
	const std::vector<float> getSightVector(int size, int degrees);
	float getCarDistanceFromMiddle() const;
	float getCarVelocity() const;
	float getCarDistanceTraveled() const;

	//---Car actions and NN interraction
	std::vector<float> getCarAction(NeuralNetwork& nn);
	float getFitness() const;
	float getFitness(double time) const;

	//---Controller actions
	std::vector<float> simulateStepForward(Car& car, float steer, float accelerate) const;
    void stepForward(); //Moves simulation; make call to NN and then ask physics to parse action
    
private:
	
	const bool debugging = true;
	const int numberOfVisionLines = 5;
	const int fieldOfView = 90; // TODO: FoV slider
	
	/***** Reward function coefficients, see reward in Controller::takeStep() *****/
	const float timeToFitnessMultiplier = 1.2;			// Unused. Fitness function balancing multiplier.
	const float wallPenalty = -10000.0f;				// Reward penalty for hitting a wall.
	const float prevVelocityCoefficient = 0.9f;			// Reward multiplier for increasing speed vs going fast. Higher value means that increasing car speed is good. Only [0.0f, 1.0f] are sensible.
	
	/***** Action-space search and learning-related constants *****/
	const unsigned int layerCount = 4;					// Don't touch or things will explode.
	const int learningMode = 1;							// Which weight adjustment algorithm to use. Currently only value 1 is supported.
	const float defaultStepSize = 0.001; 				// Learning rate; multiplies learned outcome's impact on network node weights
	const float actionDepth = 5; 						// How many variations of acceleration/turning values to test. Primary performance impact in network eval. Up to ~200 should be manageable.
	const float discountFactor = 0.1; 					// [0.0f, 1.0f); importance of "later" vs "now"
	float explorationCoefficient = 3; 					// Weighs exploration over exploitation in Q-search; decreases each step until minimum
	const float minExplorationCoefficient = 0.05;
	const float explorationCoefficientDecrease = 0.001;
	Learning trainer = Learning(defaultStepSize);
	
	// Simulation runs at 60 fps
	float32 timeStep = 1.0 / 60.0;
	int32 velocityIterations = 8;   //how strongly to correct velocity
	int32 positionIterations = 3;   //how strongly to correct position

	b2World* m_world;
	Car* currentCar;
	Track* currentTrack;
	NeuralNetwork currentNetwork;
	int stepCounter;
	float qvalue = 0.0f;
	
};

#endif
