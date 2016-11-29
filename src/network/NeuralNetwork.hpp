#include <vector>
#include "Node.hpp"
#include <tuple>

class NeuralNetwork {
public:
	NeuralNetwork();
	const int getInputSize() const;
	const int getOutputSize() const;
	const int getLayerSize(int& layer) const;
	const std::vector<double>& getOutputValues() const;
	const std::vector<double>& getOutputValuesFromInputs(const std::vector<double> values);

	void setInput(const int index, const double value);
	void setInput(const std::vector<double> values);
	void addNode(Node& node, const int type);
	void addNodes(std::vector<Node> nodes, const int type);

	void connectAll();
	void calcAll();

private:
    std::vector<int> sizes; //size of each Node-vector
	std::vector<std::vector<Node>> nodes;
};
