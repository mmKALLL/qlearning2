#ifndef QLEARNING_LEARNING_H
#define QLEARNING_LEARNING_H
#include "NeuralNetwork.hpp"
#include "Node.hpp"
#include <vector>

// SPOOKY SCARY SKELETONS
class Learning {
	friend class NeuralNetwork;

	Learning(float stepSize = 0.01);

public:
	void adjustConnection(int layer, int index, float targetValue, NeuralNetwork& nn, float stepSize);
	void adjustConnection(int layer, int index, float targetValue, NeuralNetwork& nn) {
		adjustConnection(layer, index, targetValue, nn, Controller::defaultStepSize); //call with default value
	}
	
	
	void adjustNetwork(std::vector<float> optimalAction); // Call adjustConnection on each node in network.
	
	/**
		Initial idea:
		targetValue is the value of node at [i][j] which leads to maximal fitness. That is, maximize speed as long as it doesn't
		lead to a crash or similar. For output nodes, this typically means whether we want to accelerate or not, and turn or not.
		
		gammaValue is the learning rate, a.k.a. step size of changing the weight (0.001 or something).
		
		Use in a fashion similar to getOutputValues(), by starting from output nodes and adjusting each of their weights with:
			newWeight = oldWeight + gammaValue * (targetValue - value)
		and figure out what the targetValue of each of the current node's inputs should be.
		
		To refine this idea, we should read up on backpropagation. It's the de facto method for training neural networks.
		The wikipedia article seems like a mess, though.
	*/

private:

};

#endif