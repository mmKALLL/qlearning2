#include "Learning.hpp"

Learning::Learning(float defStepSize) {
	stepSize = sSize;
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
	NeuralNetwork& nn) {

	auto node = nn.nodes[layer][index];
	float oldValue = node->calcValue();
	//float difference = targetValue - currentValue;
	//float smallestDifference = difference;
	
	
}

float Learning::getStepSize() {
	return defaultStepSize;
}

void Learning::adjustNetwork(Controller& controller, NeuralNetwork& nn, float qvalue, float qtarget, int mode) {
	if (mode == 1) {
		racistNetworkLearning(controller, nn, qvalue, qtarget); // Change this line to change learning algorithm.
	} else if (mode == 0) {
		// do nothing
	} else {
		throw "Learning.cpp: No learning algorithm with mode number " + mode + " found.";
	}
}

void Learning::racistNetworkLearning(Controller& controller, NeuralNetwork& nn, float qvalue, float qtarget) {
	float error = qtarget - qvalue;
	int layers = nn.nodes.size();
	racistNodeAdjustment(nn.nodes[layerCount - 1][0], nn, error, controller->prevWeightCoefficient, layers - 1);
}

// Recursively backpropagate weights
void racistNodeAdjustment(Node& n, NeuralNetwork& nn, float target, float prevWeightCoefficient, int currentLayer) {
	if (currentLayer > 0) {
		float inputWeightTotal = 0.0f;
		for (unsigned int i = 0; i < nn.nodes[currentLayer - 1].size(); i++) {
			inputWeightTotal += n->getWeight(i);
		}
		for (unsigned int i = 0; i < nn.nodes[currentLayer - 1].size(); i++) {
			float newWeight = ???; // prev_weight + stepsize * (sign(in_i.value) * error * (1 + abs(prev_weight) * prevWeightCoefficient))
			float nodeTarget = ???; //
			n->setWeight(i, newWeight); // currently no correction for first layers receiving multiple adjustments
			racistNodeAdjustment(nn.nodes[currentLayer - 1][i], nn, nodeTarget, prevWeightCoefficient, currentLayer - 1);
		}
	}
}