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
	const double calcValue();
	const double calcValueCascade();
	const std::vector<tuple<Node, double>> getConnectionsIn() const;
	const std::vector<Node> getConnectionsOut() const;

	void addInput(const Node& another, const double& weight = 1);
	void addOutput(const Node& another);


	std::stringstream toString() const;
private:
	bool isUpdated;
	std::vector<tuple<Node, double>> connectionsIn;
	std::vector<Node> connectionsOut;
	//int id;
	double value;
};
/*
std::ostream& operator<< (std::ostream& stream, const Node& obj) {
	return stream << obj.toString();
}*/