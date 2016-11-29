#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <sstream>
using std::tuple;


class Node {
public:
	Node(std::vector<Node> inputs = std::vector<Node>(), int type = -1);

	const double getValue() const;
	const std::vector<tuple<Node, double>> getConnectionsIn() const;
	const std::vector<Node> getConnectionsOut() const;

	const double calcValue();
	const double calcValueCascade();

	void addInput(const Node& another, const double& weight = 1);
	void addOutput(const Node& another);
	
	void setValue(const double value);
	void setWeight(const int index, const int weight);
	

	std::stringstream toString() const;
	double value;

protected:
	std::vector<tuple<Node, double>> connectionsIn;
	std::vector<Node> connectionsOut;

private:
	int type; //0=input, 1=hidden1, 2=hidden2, 3=output, -1=undefined
	//bool isUpdated;
	//int id;
};
/*
std::ostream& operator<< (std::ostream& stream, const Node& obj) {
	return stream << obj.toString();
}*/



//-------ALL BELOW IS CURRENTLY DEAD CODE--------
//
///*
//	Input layer nodes
//*/
//class InputNode : public Node {
//public:
//	InputNode() : Node() {}
//
//	//void setValue(double& const value);
//	//const double calcValue();
//	//const double calcValueCascade();
//
//	std::stringstream toString() const;
//
//private:
//};
///*
//	Hidden layer nodes
//*/
//class HiddenNode : public Node {
//public:
//	HiddenNode(std::vector<Node>& inputs = std::vector<Node>())
//		: Node(inputs) {}
//
//	std::stringstream toString() const;
//
//private:
//
//};
//
///*
//	Output layer nodes
//*/
//class OutputNode : public Node {
//public:
//	OutputNode(std::vector<Node>& inputs = std::vector<Node>())
//		: Node(inputs) {}
//
//	std::stringstream toString() const;
//
//private:
//
//};
//
////Laivap�ivitys :D//