
#include <vector>
#include "NeuralNetwork.hpp"
#include <tuple>

class Controller {
public:
	Controller();
    const Track getTrack() const;
    const Car getCar() const;
    
    void stepForward(); //Moves simulation; make call to NN and then ask physics to parse action
    
private:
    Car currentCar;
    Track currentTrack;
};