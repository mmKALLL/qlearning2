#include "NeuralNetwork.hpp"

NeuralNetwork::NeuralNetwork() {
	sizes = std::vector<int> {0, 0, 0, 0};
	//std::vector<std::vector<Node>> nodes;
}

const int NeuralNetwork::getInputSize() const {
	return sizes[0];
}

const int NeuralNetwork::getOutputSize() const {
	return sizes[3];
}

const int NeuralNetwork::getLayerSize(int& const layer) const {
	return sizes[layer];
}

//Set input for a certain node
void NeuralNetwork::setInput(int& const index, double& const value) {
	Node n = nodes[0][index];
	n.setValue(value);
}

//Set the contents of the vector as the values of input nodes. Should stop to avoid out of bounds
void NeuralNetwork::setInput(std::vector<double>& const values) {
	int size = nodes[0].size();
	for (int i = 0; i < size; i++) {
		if (i >= values.size()) { break; } //avoid out of bounds
		nodes[0][i].setValue(values[i]);
		i++;
	}
}

//Add a node to the NN
void NeuralNetwork::addNode(Node& const node, int& const type) {
	nodes[type].push_back(node);
	sizes[type] = nodes[type].size(); //update size
}

//Add a vector of nodes to the NN
void NeuralNetwork::addNode(std::vector<Node>& const nodevector, int& const type) {
	nodes[type] = nodevector;
	sizes[type] = nodes[type].size(); //update size
}

//Connect all nodes to each node in upper and lower layer
void NeuralNetwork::connectAll() {
	for (int i = 0; i < 4; i++) {
		for (auto it = nodes[i].begin(); it != nodes[i].end(); it++) {
			Node current = *it;
			if (i != 0) { //not input node, has inputs
				for (auto iti = nodes[i - 1].begin(); iti != nodes[i - 1].end(); iti++) {
					current.addInput(*iti);
				}
			}
			if (i != 3) { //not input node, has inputs
				for (auto ito = nodes[i + 1].begin(); ito != nodes[i + 1].end(); ito++) {
					current.addOutput(*ito);
				}
			}
		}
	}
}

//Calculate the value of each node, starting from hidden layer 1
void NeuralNetwork::calcAll() {
	for (int i = 1; i < 4; i++) { //inputs don't need the value calculated
		for (auto it = nodes[i].begin(); it != nodes[i].end(); it++) {
			it->calcValue();
		}
	}
}