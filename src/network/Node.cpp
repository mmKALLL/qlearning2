#include "Node.hpp"
#include <vector>
Node::Node(std::vector<Node>& inputs) {
	std::vector<tuple<Node, double>> connections;
	for (auto it = inputs.begin(); it != inputs.end(); it++) {
		//auto tmp = std::make_tuple(it, 1);
		connections.push_back(std::make_tuple(*it, 1.0));
	}
	connectionsIn = connections;
	value = 0;
	isUpdated = false;
}

const double Node::getValue() const {
	return value;
}

/*
	Non-recursively calculate the value of this node 
	without caring if inputs are old.
*/
const double Node::calcValue() {
	double val = 0;
	for (auto it = connectionsIn.begin(); it != connectionsIn.end(); it++) {
		val += std::get<0>(*it).getValue() * std::get<1>(*it);
	}
	return val;
}

//Recursively calculate the value of this node by calculating previous nodes
const double Node::calcValueCascade() {
	return 0;
}


const std::vector<tuple<Node, double>> Node::getConnectionsIn() const {
	return connectionsIn;
}

const std::vector<Node> Node::getConnectionsOut() const {
	return connectionsOut;
}

void Node::addInput(const Node& another, const double& weight) {
	connectionsIn.push_back(std::make_tuple(another, weight));
}

void Node::addOutput(const Node& another) {
	connectionsOut.push_back(another);
}


std::stringstream Node::toString() const {
	std::stringstream ss;
	ss << "Node: " << getValue();
	return ss;
}