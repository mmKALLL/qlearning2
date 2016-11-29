#include <vector>
#include "network\NeuralNetwork.hpp"
#include <tuple>
#include "car\Car.hpp"
#include "car\Track.hpp"

class Controller {
public:
	Controller();
    const Track getTrack() const;
    const Car getCar() const;

	std::vector<double>& getActionForCar(Car& car, NeuralNetwork& nn) const;

	const std::vector<float> getSightVector(int size, int degrees) const;
	const float getCarDistanceFromMiddle() const;
	const float getCarSpeed() const;
	const std::vector<float> getCarPosition() const;

	const double getFitness(double x, double y, double time) const;
	const double getFitness(Car& car, double time) const;
	const std::vector<float> Controller::simulateStepForward(Car& car, float steer, float accelerate) const;

    void stepForward(); //Moves simulation; make call to NN and then ask physics to parse action
    
private:
    Car currentCar;
    Track currentTrack;
};