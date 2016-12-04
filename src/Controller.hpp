#ifndef QLEARNING_CONTROLLER_H
#define QLEARNING_CONTROLLER_H
#include <vector>
#include <tuple>
#include "network/NeuralNetwork.hpp"
#include "network/Learning.hpp"
#include "car/Car.hpp"
#include "car/Track.hpp"

const int numberOfVisionLines = 5;
const int fieldOfView = 90; // TODO: FoV slider
const float timeToFitnessMultiplier = 1.2;
const float gammaValue = 0.001; // learning rate


class Controller {
public:
	Controller();
	void initializeRun(/*TODO: params*/); // Application launched or previous car's run completely over, so start new run
	
	//---Generic getters
    const Track getTrack() const;
    const Car getCar() const;

	//std::vector<double>& getActionForCar(Car& car, NeuralNetwork& nn) const;

	//---Getters for car
	const std::vector<float> getSightVector(Car& car, int size, int degrees) const;
	const std::vector<float> getCarPosition() const;
	float getCarDistanceFromMiddle(Car& car) const;
	float getCarVelocity(Car& car) const;
	float getCarDistanceTraveled(Car& car) const;

	//---Car actions and NN interraction
	std::vector<float> getCarAction(Car& car, NeuralNetwork& nn);
	float getFitness(Car& car) const;
	float getFitness(Car& car, double time) const;

	//---Controller actions
	std::vector<float> simulateStepForward(Car& car, float steer, float accelerate) const;
    void stepForward(); //Moves simulation; make call to NN and then ask physics to parse action
    
private:
    Car currentCar;
    Track currentTrack;
};

#endif
