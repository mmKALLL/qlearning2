#include "Node.hpp"

Node::Node(std::vector<Node>& inputs) {
	connectionsIn = inputs;
	value = 0;
}

const double Node::getValue() const {
	return value;
}