#include "NeuralNetwork.hpp"
std::random_device rd;     // only used once to initialise (seed) engine
std::mt19937 mt(rd());    // random-number engine used (Mersenne-Twister in this case)
std::uniform_real_distribution<double> rng(0.0, 1.0);

NeuralNetwork::NeuralNetwork(unsigned int layerCount) {
	//init sizes as 0
	sizes = std::vector<unsigned int> {0, 0, 0, 0};
	//std::vector<std::vector<Node>> nodes;// (layerCount, std::vector<Node>(0));
	
	for (unsigned int i = 0; i < layerCount; i++) {
		nodes.push_back(std::vector<Node*>{});
	} // FIXME: Esa; make it work so that it builds an appropriate network for the new system
}

int NeuralNetwork::getInputSize() const {
	return sizes[0];
}

int NeuralNetwork::getOutputSize() const {
	return sizes[ nodes.size()-1 ];
}

//Layer of hidden ie 0 or 1
int NeuralNetwork::getLayerSize(int layer) const {
	if (layer == 0) return sizes[1];
	else if (layer == 1) return sizes[2];
	else throw "Invalid hidden layer index, use 0 or 1";
}

//Returns vector of the values of output layer nodes
std::vector<float> NeuralNetwork::getOutputValues() const {
	std::vector<float> values;
	int oli = nodes.size() - 1; //output layer index
	if (oli < 0)
		throw "NeuralNetwork::getOutputValues Neural Networks needs at least two layers";
	for (auto it = nodes[oli].begin(); it != nodes[oli].end(); it++) {
		values.push_back((*it)->getValue());
	}
	return values;
}

// FIXME: Doesn't work under the new q-learning system?
//Set new inputs, re-calculate and return outputs
std::vector<float> NeuralNetwork::getOutputValuesFromInputs (std::vector<float> values, bool useSig) {
	if (values.size() != nodes[0].size())
		throw "Function input size needs to match number of input nodes";
	setInputs(values);
	if (useSig) calcAllSig(); else calcAll();
	return getOutputValues();
}

// Get action based on state. Action depth tells how many action combinations to evaluate.
// Use only odd numbers. Exploration coefficient weighs exploration; lower it to increase exploitation.
std::vector<float> NeuralNetwork::getAction(std::vector<float> state, unsigned int actionDepth, float explorationCoefficient, bool useSig) {
	std::cout << "Sizes: " << state.size() << " " << extraInputs << std::endl;
	std::cout << "Nodes 0 : " << nodes[0].size() << std::endl;
	if (state.size() + extraInputs != nodes[0].size()) {
		throw "Wrong size of input vector in NeuralNetwork::getAction(). Did you forget to update Controller::stateSize and NeuralNetwork::extraInputs?";
	}
	
	std::vector<float> qvalues;
	std::vector<float> result;
	
	// Try uniform combinations of actions, store network's Q-value approximations for each of them
	for (float acc = -1; acc <= 1.00001; acc += (2.0f / (actionDepth - 1))) {
		for (float turn = -1; turn <= 1.00001; turn += (2.0f / (actionDepth - 1))) {
			std::vector<float> input = state;
			input.push_back(acc);
			input.push_back(turn);
			input.push_back(1.0f); // bias input
			std::vector<float> outputValues = getOutputValuesFromInputs(input, useSig);
			if (outputValues.size() != 1) {
				throw "Invalid amount of output nodes in NeuralNetwork::getAction().";
			} else {
				qvalues.push_back(outputValues[0]);
			}
		}
	}
	
	// Boltzmann probability distribution which weighs exploration over exploitation early on
	std::vector<float> actionProbabilities;
	float quotient = 0.0f;
	std::cout << "qvalues size: " << qvalues.size() << std::endl;
	for (float& x : qvalues) {
		std::cout << "qvalue: " << x << std::endl;
		quotient += exp((x / 1000.0) / explorationCoefficient); // Divide by 1000 to prevent exponential growth for abs(qvalue) > 1.
	}
	for (float& x : qvalues) {
		std::cout << "        Boltzmann action odds!!: " << exp(x / explorationCoefficient) << std::endl;
		actionProbabilities.push_back(
			exp((x / 1000.0) / explorationCoefficient) / quotient
		);
	}
	std::cout << "            Boltzmann quotient!!!: " << quotient << std::endl;

	float probSum = 0.0f;
	for (float& x : actionProbabilities) {
		std::cout << "   actionProbability " << x << std::endl;
		probSum += x;
	}
	
	// Choose an action from the distribution
	double probTarget = rng(mt) * probSum;
	std::cout << "AP size " << actionProbabilities.size() << " and sum: " << probSum << std::endl;
	std::cout << "probTarget before decrease: " << probTarget << std::endl;
	for (unsigned int i = 0; i < actionProbabilities.size(); i++) {
		probTarget -= actionProbabilities[i];
		std::cout << "probTarget before decrease: " << probTarget << std::endl;
		if (probTarget <= 0.0f) {
			result.push_back(-1.0f + (i / actionDepth) * (2.0f / (actionDepth - 1))); 	// acceleration
			result.push_back(-1.0f + (i % actionDepth) * (2.0f / (actionDepth - 1))); 	// turning rate
			result.push_back(qvalues[i]);												// Q-value
			
			// Leave network in the state it was with that input
			std::vector<float> input = state;
			input.push_back(result[0]);
			input.push_back(result[1]);
			input.push_back(1.0f); // bias input
			getOutputValuesFromInputs(input, useSig);
			return result;
		}
	}
	std::cout << "probTarget after decrease: " << probTarget << std::endl;
	throw "Not enough actions tried! Issues with actionProbabilities in NeuralNetwork::getAction(). Most likely sum of Q-values overflowed or Boltzmann distribution quotient is too large; try a smaller multiplier.";
	
}


//Set input for a certain node
void NeuralNetwork::setInput(const int index, const float value) {
	Node* n = nodes[0][index];
	n->setValue(value);
}

//Set the contents of the vector as the values of input nodes. Should stop to avoid out of bounds
void NeuralNetwork::setInputs(std::vector<float>& values) {
	unsigned int size = nodes[0].size();
	if (values.size() > size) { throw "NeuralNetwork::setInputs input vector larger than amount of input nodes"; }
	for (unsigned int i = 0; i < size; i++) {
		nodes[0][i]->setValue(values[i]);
	}
}

//Add a node to the NN
void NeuralNetwork::addNode(Node& node, const int type) {
	nodes[type].push_back(&node);
	sizes[type] = nodes[type].size(); //update size
}

//Add a vector of nodes to the NN
void NeuralNetwork::addNodes(std::vector<Node*>& nodevector, const int type) {
	nodes[type] = nodevector;
	sizes[type] = nodes[type].size(); //update size
}


/*
	Build a Neural Network from scratch:
		-Creates node-objects
		-Sets them a "unique" ID
		-Adds nodes to the network data structure
		-Connects all nodes
		-Randomizes weights if requested to
*/
void NeuralNetwork::build(std::vector<unsigned int> layerSizes, bool rand, float low, float high) {
	//if (layerSizes.size() != 4) throw "NeuralNetwork::build invalid layerSizes vector, should have lenght of 4";
	int id = 0;
	for (unsigned int i = 0; i < layerSizes.size(); i++) {
		for (unsigned int j = 0; j < layerSizes[i]; j++) {
			Node* newNode = new Node(id);//set "unique" id to each node
			id++;
			addNode(*newNode, i);
		}
	}
	connectAll();
	if(rand) randomize(low, high);
}

/*
	Connect all nodes in the Neural Network such that any given node
	connects to every node in the layer above and below it.
	Ie every hidden layer 1 node connects to every input node and
	every hidden layer 2 node.
*/
void NeuralNetwork::connectAll() {
	for (unsigned int i = 0; i < nodes.size(); i++) { //For each layer
		for (auto it = nodes[i].begin(); it != nodes[i].end(); it++) { //For each node
			Node* current = *it;
			if (i != 0) { //not input node, has inputs
				for (auto iti = nodes[i - 1].begin(); iti != nodes[i - 1].end(); iti++) {
					current->addInput(**iti);
				}
			}
			if (i != nodes.size() - 1) { //not output node, has outputs
				for (auto ito = nodes[i + 1].begin(); ito != nodes[i + 1].end(); ito++) {
					current->addOutput(**ito);
				}
			}
		}
	}
}

//Calculate the value of each node, starting from hidden layer 1
void NeuralNetwork::calcAll() {
	for (unsigned int i = 1; i < nodes.size(); i++) { //inputs don't need the value calculated, i=1
		for (auto it = nodes[i].begin(); it != nodes[i].end(); it++) {
			(*it)->calcValue();
		}
	}
}

//Calculate the value of each node, starting from hidden layer 1, use Sigmoid function
void NeuralNetwork::calcAllSig() {
	for (unsigned int i = 1; i < nodes.size(); i++) { //inputs don't need the value calculated, i=1
		for (auto it = nodes[i].begin(); it != nodes[i].end(); it++) {
			(*it)->calcValueSig();
		}
	}
}

//A way to initialize the network: Randomize all weights and calc values
void NeuralNetwork::randomize(float low, float high) {
	for (auto nodeVector : nodes) { // for each layer
		for (Node* node : nodeVector) { // for each node
			for (unsigned int i = 0; i < node->getConnectionsIn().size(); i++) { // for each connection
				//lol no float rnd = low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
				float rnd = low + rng(mt) * (high - low);
				node->setWeight(i, rnd);
			}
		}
	}
	calcAll();
}
