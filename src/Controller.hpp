
#include <vector>
#include <tuple>
#include "network/NeuralNetwork.hpp"
#include "car/Car.hpp"
#include "car/Track.hpp"

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

    void stepForward(); //Moves simulation; make call to NN and then ask physics to parse action
    
private:
    Car currentCar;
    Track currentTrack;
};