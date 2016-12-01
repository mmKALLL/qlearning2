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

class Controller {
public:
	Controller();
    const Track& getTrack() const;
    const Car& getCar() const;

	std::vector<double>& getActionForCar(Car& car, NeuralNetwork& nn) const;

	const std::vector<float> getSightVector(int size, int degrees) const;
	float getCarDistanceFromMiddle() const;
	float getCarVelocity() const;
	const std::vector<float> getCarPosition() const;

	const std::vector<double> getCarAction();
	double getFitness(double x, double y, double time) const;
	double getFitness(Car& car, double time) const;
	const std::vector<float> simulateStepForward(Car& car, float steer, float accelerate) const;

    void stepForward(); //Moves simulation; make call to NN and then ask physics to parse action
    
private:
    Car currentCar;
    Track currentTrack;
};

#endif
