#include "Learning.hpp"

Learning::Learning(float sSize) {
	defaultStepSize = sSize;
}


//FIXME:
//SPOOKY SKELETONS FOR REAL FLESHY FUNCTIONS
void evolve(NeuralNetwork& nn) { //???

}

//FIXME:
//SPOOKY SKELLIES FOR REAL FLESHY FUNCTIONS
void adjustWeights() {

}

//FIXME:
//Adjust connections to this node to get its value closer to target
void Learning::adjustConnection(int layer, int index, float targetValue,
								NeuralNetwork& nn, float stepSize) {

	auto node = nn.nodes[layer][index];
	float oldValue = node->calcValue();
	//float difference = targetValue - currentValue;
	//float smallestDifference = difference;


}

//Adjust connections to this node to get its value closer to target
//Simple dummy algorithm to get somehow going
void Learning::adjustConnectionSimple(int layer, int index, float targetValue,
	NeuralNetwork& nn, float stepSize) {

	auto node = nn.nodes[layer][index];
	float oldValue = node->calcValue();
	//float difference = targetValue - currentValue;
	//float smallestDifference = difference;
	
	
}

float Learning::getStepSize() {
	return defaultStepSize;
}

void Learning::adjustNetwork(NeuralNetwork& nn, float qvalue, float qtarget, int mode) {
	if (mode == 1) {
		racistNetworkLearning(nn, qvalue, qtarget); // Change this line to change learning algorithm.
	} else {
		throw "Learning.cpp: No learning algorithm with mode number " + mode + " found.";
	}
}

void Learning::racistNetworkLearning(NeuralNetwork& nn, float qvalue, float qtarget) {
	
}