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
    const Track& getTrack() const;
    const Car& getCar() const;

	//std::vector<double>& getActionForCar(Car& car, NeuralNetwork& nn) const;

	//---Getters for car
	const std::vector<float> getSightVector(int size, int degrees);
	const std::vector<float> getCarPosition() const;
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
	const float timeToFitnessMultiplier = 1.2;
	const float defaultStepSize = 0.001; // learning rate
	const unsigned int layerCount = 4;
	const Learning teacher = Learning(defaultStepSize);
	
	// Simulation runs at 60 fps
	float32 timeStep = 1 / 60.0;
	int32 velocityIterations = 8;   //how strongly to correct velocity
	int32 positionIterations = 3;   //how strongly to correct position

	
	b2World* world = new b2World(b2Vec2(0,0));
	Track* currentTrack;
	Car currentCar;
	NeuralNetwork currentNetwork;
	int stepCounter;
};

#endif
