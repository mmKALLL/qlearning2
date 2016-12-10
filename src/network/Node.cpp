#include "Node.hpp"

Node::Node(int idd, std::vector<Node> inputs, int t) {
	std::vector<std::tuple<Node, float>> connections;
	for (auto it = inputs.begin(); it != inputs.end(); it++) {
		//auto tmp = std::make_tuple(it, 1);
		connections.push_back(std::make_tuple(*it, 1.0));
	}
	id = idd;
	connectionsIn = connections;
	value = 0;
	type = t;
	//isUpdated = false;
}

float Node::getValue() const {
	return value;
}

int Node::getType() const {
	return type;
}

const int Node::getID() const {
	return id;
}

const std::vector<std::tuple<Node, float>>& Node::getConnectionsIn() const {
	return connectionsIn;
}

const std::vector<Node>& Node::getConnectionsOut() const {
	return connectionsOut;
}

const std::tuple<Node, float>& Node::getConnection(int index) const {
	return connectionsIn[index];
}

/*
	Non-recursively calculate the value of this node 
	without caring if inputs are old.
*/
float Node::calcValue() {
	float val = 0;
	for (auto it = connectionsIn.begin(); it != connectionsIn.end(); it++) {
		val += std::get<0>(*it).getValue() * std::get<1>(*it);
	}
	//isUpdated = true;
	value = val; // Save it
	return val;
}

// Calculate the value of this node, put it through a Sigmoid function and save it
float Node::calcValueSig() {
	float val = 0;
	for (auto it = connectionsIn.begin(); it != connectionsIn.end(); it++) {
		val += std::get<0>(*it).getValue() * std::get<1>(*it);
	}
	//isUpdated = true;
	val = fastSigmoid(val);
	value = val; // Save it
	return val;
}

//Recursively calculate the value of this node by calculating previous nodes
float Node::calcValueCascade() {
	float val = 0;
	for (auto it = connectionsIn.begin(); it != connectionsIn.end(); it++) {
		val += std::get<0>(*it).calcValueCascade() * std::get<1>(*it);
	}
	//isUpdated = true;
	value = val;
	return val;
}

void Node::addInput(const Node& another, const float& weight) {
	connectionsIn.push_back(std::make_tuple(another, weight));
	//isUpdated = false;
}

void Node::addOutput(const Node& another) {
	connectionsOut.push_back(another);
	//isUpdated = false;
}

void Node::setID(int newID) {
	id = newID;
}

void Node::setValue(const float val) {
	value = val;
}

//Set weight of connection at index to weight
void Node::setWeight(const unsigned int index, const float weight) {
	if (index < connectionsIn.size()) {
		std::get<1>(connectionsIn[index]) = weight;
	}
}

//------------toString--------------
std::string Node::toString() const {
	std::string ss = "Node: ";
	return ss + std::to_string(getValue());
}


float fastSigmoid(float input) {
	return input / (1 + abs(input));
}