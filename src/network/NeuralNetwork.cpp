#include "NeuralNetwork.hpp"

NeuralNetwork::NeuralNetwork() {
	//init sizes as 0
	sizes = std::vector<int> {0, 0, 0, 0};
	std::vector<std::vector<Node>> nodes;
}

const int NeuralNetwork::getInputSize() const {
	return sizes[0];
}

const int NeuralNetwork::getOutputSize() const {
	return sizes[ nodes.size()-1 ];
}

//Layer of hidden ie 0 or 1
const int NeuralNetwork::getLayerSize(int& const layer) const {
	if (layer == 0) return sizes[1];
	else if (layer == 1) return sizes[2];
	else throw "Invalid hidden layer index, use 0 or 1";
}

//Returns vector of the values of output layer nodes
const std::vector<double>& NeuralNetwork::getOutputs() const {
	std::vector<double> values;
	int hli = nodes.size() - 1; //hidden layer index
	for (auto it = nodes[hli].begin(); it != nodes[hli].end(); it++) {
		values.push_back(it->getValue());
	}
	return values;
}

//Set new inputs, re-calculate and return outputs
const std::vector<double>& NeuralNetwork::getOutputsFromInputs
								(std::vector<double>& const values) {
	if (values.size() != nodes[0].size())
		throw "Function input size needs to match number of input nodes";
	setInput(values);
	calcAll();
	return getOutputs();
}



//Set input for a certain node
void NeuralNetwork::setInput(int& const index, double& const value) {
	Node n = nodes[0][index];
	n.setValue(value);
}

//Set the contents of the vector as the values of input nodes. Should stop to avoid out of bounds
void NeuralNetwork::setInput(std::vector<double>& const values) {
	int size = nodes[0].size();
	if (values.size >= size) { throw "NN input vector larger than amount of input nodes"; }
	for (int i = 0; i < size; i++) {
		nodes[0][i].setValue(values[i]);
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
	for (int i = 0; i < nodes.size(); i++) { // TODO: replace this and the 4 below with len(nodes)? Does that work?
		for (auto it = nodes[i].begin(); it != nodes[i].end(); it++) {
			Node current = *it;
			if (i != 0) { //not input node, has inputs
				for (auto iti = nodes[i - 1].begin(); iti != nodes[i - 1].end(); iti++) {
					current.addInput(*iti);
				}
			}
			if (i != nodes.size() - 1) { //not input node, has inputs
				for (auto ito = nodes[i + 1].begin(); ito != nodes[i + 1].end(); ito++) {
					current.addOutput(*ito);
				}
			}
		}
	}
}

//Calculate the value of each node, starting from hidden layer 1
void NeuralNetwork::calcAll() {
	for (int i = 1; i < nodes.size(); i++) { //inputs don't need the value calculated, i=1
		for (auto it = nodes[i].begin(); it != nodes[i].end(); it++) {
			it->calcValue();
		}
	}
}