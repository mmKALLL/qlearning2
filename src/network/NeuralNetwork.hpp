
#include <vector>
#include "Node.hpp"
#include <tuple>

class NeuralNetwork {
public:
	NeuralNetwork();
	const int getInputSize() const;
	const int getOutputSize() const;
	const int getLayerSize(int& const layer) const;
	const std::vector<double>& getOutputs() const;

	void setInput(int& const index, double& const value);
	void setInput(std::vector<double>& const values);
	void addNode(Node& const node, int& const type);
	void addNode(std::vector<Node>& const nodes, int& const type);

	void connectAll();
	void calcAll();

private:
    std::vector<int> sizes; //size of each Node-vector(?)
	std::vector<std::vector<Node>> nodes;
};
