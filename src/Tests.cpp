#include "Tests.hpp"
#include "network\NeuralNetwork.hpp"
#include <vector>
#include <tuple>

using std::cout;
using std::endl;
using std::string;

typedef int(*FC)(); //FC for function call

const string separator = string("");

//Easy way to create new tests, takes function call as parameter
void runTest(FC test, std::string& name, int& passed, int& failed) {
	//cout << "========" << endl;
	cout << "Running " << name << " test";
	if (test() == 0) {
		cout << "  .. passed!" << endl;
		passed++;
	}
	else {
		cout << separator << endl;
		cout << name << " test failed!" << endl;
		cout << separator << endl;
		failed++;
	}
}

//Helper function to reduce typing
template<typename T>
bool assert(T value, T correct, string valDesc = "", string corrDesc = "") {
	if (value == correct) {
		return true;
	}
	else {
		cout << endl << separator <<endl;
		cout << valDesc << "value is: " << value << endl;
		cout << corrDesc << "should be: " << correct << endl;
		return false;
	}
}
template<typename T>
int testAssert(T value, T correct, string valDesc = "", string corrDesc = "") {
	if (assert<T>(value, correct, valDesc, corrDesc)) {
		return 0;
	}
	else return -1;
}


/* ------------------------------------------------------------
						Network Tests
-------------------------------------------------------------*/

int runNetworkTest() {
	//Node testNode();
	std::cout << "Running NeuralNetwork tests" << std::endl;

	//---Create---
	NeuralNetwork network = NeuralNetwork();
	network.build(std::vector<unsigned int> {3, 4, 3, 2});
	std::cout << "Created a NeuralNetwork" << std::endl;

	std::cout << "Setting input nodes and calculating..." << std::endl;
	auto outputs = network.getOutputValuesFromInputs(std::vector<float> {-0.5, 0.5, 1.0});
	std::cout << "Outputs are: ";
	for (auto output : outputs) {
		std::cout << output << " ";
	}
	std::cout << std::endl;
	return 0;
}

int simpleNetworkTest() {
	NeuralNetwork network = NeuralNetwork(2);
	network.build(std::vector<unsigned int> {2, 1}, false);
	auto outputs = network.getOutputValuesFromInputs(std::vector<float> {1, 0.5});
	std::cout << "Outputs are: ";
	for (auto output : outputs) {
		std::cout << output << " ";
	}
	cout << endl;
	if (outputs[0] == 0) return 1;
	else return 0;
}


/* ------------------------------------------------------------
						Node Tests
-------------------------------------------------------------*/

int connectNodes() {
	Node input = Node(0);
	Node output = Node(1);

	output.addInput(input);
	std::tuple<Node, float> tuple = output.getConnection(0);
	int id1 = std::get<0>(tuple).getID();
	int id2 = input.getID();

	if (assert<int>(id1, id2)) {
		return 0;
	}
	return -1;
}

int setNodeWeight() {
	Node input = Node();
	Node output = Node();

	output.addInput(input);
	output.setWeight(0, 1);
	auto connection = output.getConnection(0);
	if (assert<float>(std::get<1>(connection), 1) ) {
		return 0;
	}
	return -1;
}

int readNodeValue() {
	Node node = Node();
	node.setValue(2);
	return testAssert<float>(node.getValue(), 2);
}
int calculateNodeValue() {
	Node input1 = Node();
	Node input2 = Node();
	Node output = Node();

	output.addInput(input1);
	output.addInput(input1);
	
	output.setWeight(0, 1);
	output.setWeight(1, -2);

	input1.setValue(1);
	input2.setValue(2);

	output.calcValue();

	if (assert<float>(output.getValue(), -3)) {
		return 0;
	}
	return -1;
}

/* ------------------------------------------------------------
						Physics Tests
-------------------------------------------------------------*/
int runPhysicsTest() {
	return 0;
}

/*
	Run all the tests!
*/
int runTests() {
    int passed = 0;
	int failed = 0;

	runTest(connectNodes, string("Connect nodes"), passed, failed);
	runTest(calculateNodeValue, string("Calculate node value"), passed, failed);
	runTest(setNodeWeight, string("Set connection weight"), passed, failed);
	runTest(readNodeValue, string("Read value of a node"), passed, failed)


	runTest(runNetworkTest, string("Network"), passed, failed);
	runTest(simpleNetworkTest, string("Simple Network"), passed, failed);
	runTest(runPhysicsTest, string("Physics"), passed, failed);

	cout << "=====" << "Passed: " << passed << ", failed: " << failed << "=====" << endl;
	if (failed == 0) {
		std::cout << std::endl << "All tests were OK." << std::endl;
	}
	else {
		cout << endl << "Some tests failed." << endl;
	}
	
	return 0;
}

