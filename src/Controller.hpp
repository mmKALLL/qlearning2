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


class Controller {
public:
	Controller();

	//---Generic getters
    const Track getTrack() const;
    const Car getCar() const;

	//std::vector<double>& getActionForCar(Car& car, NeuralNetwork& nn) const;

	//---Getters for car
	const std::vector<float> getSightVector(int size, int degrees) const;
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
    Car currentCar;
    Track currentTrack;
};

#endif
