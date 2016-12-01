#include "Node.hpp"
#include <vector>

Node::Node(std::vector<Node> inputs, int t) {
	std::vector<std::tuple<Node, float>> connections;
	for (auto it = inputs.begin(); it != inputs.end(); it++) {
		//auto tmp = std::make_tuple(it, 1);
		connections.push_back(std::make_tuple(*it, 1.0));
	}
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

const std::vector<std::tuple<Node, float>> Node::getConnectionsIn() const {
	return connectionsIn;
}

const std::vector<Node> Node::getConnectionsOut() const {
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
	return val;
}

//Recursively calculate the value of this node by calculating previous nodes
float Node::calcValueCascade() {
	float val = 0;
	for (auto it = connectionsIn.begin(); it != connectionsIn.end(); it++) {
		val += std::get<0>(*it).calcValueCascade() * std::get<1>(*it);
	}
	//isUpdated = true;
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

void Node::setValue(const float val) {
	value = val;
}

//Set weight of connection at index to weight
void Node::setWeight(const int index, const int weight) {
	if (index < connectionsIn.size()) {
		std::get<1>(connectionsIn[index]) = weight;
	}
}

//------------toString--------------
std::stringstream Node::toString() const {
	std::stringstream ss;
	ss << "Node: " << getValue();
	return ss;
}
