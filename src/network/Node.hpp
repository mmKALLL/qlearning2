#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <sstream>
using std::tuple;


class Node {
public:
	Node(std::vector<Node>& inputs = std::vector<Node>());

	const double getValue() const;
	const std::vector<tuple<Node, double>> getConnectionsIn() const;
	const std::vector<Node> getConnectionsOut() const;

	const double calcValue();
	const double calcValueCascade();

	void addInput(const Node& another, const double& weight = 1);
	void addOutput(const Node& another);

	std::stringstream toString() const;
	double value;

protected:
	std::vector<tuple<Node, double>> connectionsIn;
	std::vector<Node> connectionsOut;

private:
	//bool isUpdated;
	//int id;
};
/*
std::ostream& operator<< (std::ostream& stream, const Node& obj) {
	return stream << obj.toString();
}*/





/*
	Input layer nodes
*/
class InputNode : public Node {
public:
	InputNode() : Node() {}

	void setValue(double& const value);
	const double calcValue();
	const double calcValueCascade();

	std::stringstream toString() const;

private:
};
/*
	Hidden layer nodes
*/
class HiddenNode : public Node {
public:
	HiddenNode(std::vector<Node>& inputs = std::vector<Node>())
		: Node(inputs) {}

	std::stringstream toString() const;

private:
	
};

/*
	Output layer nodes
*/
class OutputNode : public Node {
public:
	OutputNode(std::vector<Node>& inputs = std::vector<Node>())
		: Node(inputs) {}

	std::stringstream toString() const;

private:
	
};