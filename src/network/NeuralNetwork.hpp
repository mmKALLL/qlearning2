#ifndef QLEARNING_NEURALNETWORK_H
#define QLEARNING_NEURALNETWORK_H
#include <vector>
#include "Node.hpp"
#include <tuple>

class NeuralNetwork {
	friend class Learning;
	friend class tests;
public:
	NeuralNetwork(unsigned int layerCount = 4);
	int getInputSize() const;
	int getOutputSize() const;
	int getLayerSize(int layer) const;
	std::vector<float> getOutputValues() const;
	std::vector<float> getOutputValuesFromInputs(std::vector<float> inputs, bool useSig = false);

	void setInput(const int index, const float value);
	void setInputs(std::vector<float>& values);
	void addNode(Node& const node, const int type);
	void addNodes(std::vector<Node*>& nodes, const int type);

	void build(std::vector<unsigned int> layerSizes, bool randomize = true, float low = -0.1, float high = 0.1);
	void connectAll();
	void calcAll();
	void calcAllSig();
	void randomize(float low, float high);

protected:
	std::vector<std::vector<Node*>> nodes;
	std::vector<unsigned int> sizes; //size of each Node-vector

private:
	
};

#endif