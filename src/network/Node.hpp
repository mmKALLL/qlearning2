#include <string>
#include <vector>

class Node {
public:
	Node(std::vector<Node>& inputs = std::vector<Node>());

	const double getValue() const;
	const double calcValue();
	const std::vector<Node> GetConnectionsIn() const;
	const std::vector<Node> GetConnectionsOut() const;

	void connectTo(Node& const another);


	const std::string toString() const;
private:
	std::vector<Node> connectionsIn;
	std::vector<Node> connectionsOut;
	//int id;
	double value;
};