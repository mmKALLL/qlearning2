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
	float oldValue = node.calcValue();
	//float difference = targetValue - currentValue;
	//float smallestDifference = difference;


}