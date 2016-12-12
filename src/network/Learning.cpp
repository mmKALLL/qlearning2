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
	return this.defaultStepSize;
}

void Learning::adjustNetwork(/*std::vector<float> optimalAction*/)
{
	
}
