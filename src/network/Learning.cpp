#include "Learning.hpp"

Learning::Learning(float defStepSize) : stepSize(defStepSize) {
	
}


//Adjust connections to this node to get its value closer to target
//Simple dummy algorithm to get somehow going
void Learning::adjustConnectionSimple(int layer, int index, float targetValue, NeuralNetwork& nn) {

	auto node = nn.nodes[layer][index];
	float oldValue = node->calcValue();
	//float difference = targetValue - currentValue;
	//float smallestDifference = difference;
	
	
}

float Learning::getStepSize() {
	return stepSize;
}

void Learning::adjustNetwork(Controller& controller, NeuralNetwork& nn, float qvalue, float qtarget, int mode) {
	if (mode == 2) {
		throw "Learning mode 2 is not yet supported. Be back soon!";
		backPropagationLearning(controller, nn, qvalue, qtarget);
	} else if (mode == 1) {
		racistNetworkLearning(controller, nn, qvalue, qtarget);
	} else if (mode == 0) {
		// do nothing
	} else {
		throw std::string("Learning.cpp: No learning algorithm with mode number ") + std::to_string(mode) + std::string(" found.");
	}
}

void Learning::racistNetworkLearning(Controller& controller, NeuralNetwork& nn, float qvalue, float qtarget) {
	int layers = nn.nodes.size();
	
	// Debug code
	Node& n = *nn.nodes[controller.layerCount - 1][0];
	std::cout << "inputValueSign for output: " << (n.getValue() > 0) - (n.getValue() < 0) << std::endl;
	
	
	racistNodeAdjustment(controller, *nn.nodes[controller.layerCount - 1][0], nn, qtarget, controller.prevWeightCoefficient, layers - 1);
	
}



// Recursively adjust weights
void Learning::racistNodeAdjustment(Controller& controller, Node& n, NeuralNetwork& nn, float target, float prevWeightCoefficient, int currentLayer) {
	if (currentLayer > 0) {
		float inputWeightTotal = 0.0f;
		for (unsigned int i = 0; i < nn.nodes[currentLayer - 1].size(); i++) {
			inputWeightTotal += n.getWeight(i);
		}

		float error = n.getValue() - target;
		std::cout << "       target is: " << target <<  ", error is: " << error << std::endl;
		for (unsigned int i = 0; i < nn.nodes[currentLayer - 1].size(); i++) {
			Node& input = *std::get<0>(n.getConnectionsIn()[i]);
			int inputValueSign = (input.getValue() > 0) - (input.getValue() < 0);
			
			// Magic happens. Pseudocode and then real code. Painstakingly implemented a pseudo-backpropagation from scratch. Because I can.
			// prev_weight + stepsize * (sign(in_i.value) * error * (1 + abs(prev_weight) * prevWeightCoefficient))
			float newWeight = n.getWeight(i) + stepSize * (inputValueSign * error * (1 + abs(n.getWeight(i)) * prevWeightCoefficient));
			newWeight = max(-10.0f, newWeight);
			newWeight = min(10.0f, newWeight);
			// prev_value * prevValueCoefficient + (1 - prevValueCoefficient) * (target / prev_weight) / nn.nodes[currentLayer - 1].size()
			float nodeTarget = input.getValue() * controller.prevValueCoefficient + (1 - controller.prevValueCoefficient) * (target / n.getWeight(i)) / nn.nodes[currentLayer - 1].size();
			racistNodeAdjustment(controller, *nn.nodes[currentLayer - 1][i], nn, nodeTarget, prevWeightCoefficient, currentLayer - 1);
			n.setWeight(i, newWeight); // TODO: currently very little correction for first layers receiving multiple adjustments
		}
	}
}

void Learning::backPropagationLearning(Controller& controller, NeuralNetwork& nn, float qvalue, float qtarget) {
	// TODO: Implement more typical BP algorithm if there is extra time.
}