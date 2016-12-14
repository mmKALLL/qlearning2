#ifndef QLEARNING_LEARNING_H
#define QLEARNING_LEARNING_H
#include "NeuralNetwork.hpp"
#include "Node.hpp"
#include <vector>
#include <tuple>
class Controller;
#include "../Controller.hpp"

// SPOOKY SCARY SKELETONS
class Learning {
	friend class NeuralNetwork;

public:
	Learning(float defStepSize = 0.01);

	float getStepSize();
	
	// Dummy function
	void adjustConnectionSimple(int layer, int index, float targetValue, NeuralNetwork& nn);

	// New and shiny stuff
	void adjustNetwork(Controller& controller, NeuralNetwork& nn, float qvalue, float qtarget, int mode = 1);

	void racistNetworkLearning(Controller& controller, NeuralNetwork& nn, float qvalue, float qtarget);

	void racistNodeAdjustment(Node& n, NeuralNetwork& nn, float target, float prevWeightCoefficient, int currentLayer);
	
	void backPropagationLearning(Controller& controller, NeuralNetwork& nn, float qvalue, float qtarget);
	
	/**
		Basically we are going to implement this:
		https://www.cs.swarthmore.edu/~meeden/cs81/s12/papers/MarkStevePaper.pdf
		
		And then this:
		https://mattmazur.com/2015/03/17/a-step-by-step-backpropagation-example/
	
		A scrapped initial idea:
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
	float stepSize;
};

#endif