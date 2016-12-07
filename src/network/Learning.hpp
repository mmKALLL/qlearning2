#ifndef QLEARNING_LEARNING_H
#define QLEARNING_LEARNING_H
#include "NeuralNetwork.hpp"
#include "Node.hpp"
#include <vector>


//SPOOKY SCARY SKELETON FOR A REAL CLASS
class Learning { //???
	friend class NeuralNetwork;

	Learning(float stepSize = 0.01);

public:
	void adjustConnection(int layer, int index, float targetValue, NeuralNetwork& nn, float stepSize = defaultStepSize);

private:
	float defaultStepSize; //How much connections will be adjusted at a time
};

#endif