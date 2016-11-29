
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

    void stepForward(); //Moves simulation; make call to NN and then ask physics to parse action
    
private:
    Car currentCar;
    Track currentTrack;
};