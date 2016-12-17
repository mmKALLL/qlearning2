#ifndef QLEARNING_FILEREADER_H
#define QLEARNING_FILEREADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <utility>

using std::string;
using std::cout;
using std::endl;
using std::vector;



class FileReader {
public:
	FileReader();

	void readFile(string name);

	void init();
	void test(); // Some unit tests. Here because I'm lazy

	//Helpers
	string cut(string& str);
	std::vector<string>& split(string& str);

	std::map<string, float> parsedFloat;
	std::map<string, bool> parsedBool;
	std::map<string, int> parsedInt;
private:
	int lineNumber = 0;
	const string filename = string("constants.txt");
};



#endif
