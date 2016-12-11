#ifndef QLEARNING_NODE_H
#define QLEARNING_NODE_H
#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <sstream>


class Node {
public:
	Node(int id = -1, std::vector<Node> inputs = std::vector<Node>(), int type = -1);

	float getValue() const;
	int getType() const;
	const int getID() const;
	const std::vector<std::tuple<Node, float>>& getConnectionsIn() const;
	const std::vector<Node>& getConnectionsOut() const;
	const std::tuple<Node, float>& getConnection(int index) const;

	float calcValue();
	float calcValueSig(); // Value also put through Sigmoid function
	float calcValueCascade();

	void addInput(Node& another, const float& weight = 1);
	void addOutput(Node& another);
	
	void setID(int id);
	void setValue(float value);
	void setWeight(const unsigned int index, const float weight);
	

	std::string toString() const;
	float value;

protected:
	std::vector<std::tuple<Node, float>> connectionsIn;
	std::vector<Node> connectionsOut;

private:
	int type; //0=input, 1=hidden1, 2=hidden2, 3=output, -1=undefined
	int id;
	//bool isUpdated;
	//int id;
};

float fastSigmoid(float input);
/*
std::ostream& operator<< (std::ostream& stream, const Node& obj) {
	return stream << obj.toString();
}*/

#endif