#include "FileReader.hpp"


FileReader::FileReader()
{
	this->init();
}

void FileReader::readFile(string name) {
	std::ifstream file(name);

	if (!file.is_open()) { // The file could not be opened
		throw "FileReader: File could not be opened";
	}
	else { // File can be safely used
		std::map<string, float> parsedFloat;
		std::map<string, bool> parsedBool;
		std::map<string, int> parsedInt;

		string line;
		while (std::getline(file, line)) {
			vector<string> subs = split(line);
			if (subs[0].at(0) != '/' && 
				subs[2].at(0) == '=' &&
				subs[0].at(0) != '\n') {
				//Line is valid
				//Form is typename variablename = value
				string temp = cut(subs[3]);
				//Parse! :D
				if (subs[0] == string("float")) {
					parsedFloat.insert(std::make_pair(subs[1], float(std::stoi(temp))));
				} 
				else if (subs[0] == string("bool")) {
					if (temp == string("true")) parsedBool.insert(std::make_pair(subs[1], true));
					else if (temp == string("false")) parsedBool.insert(std::make_pair(subs[1], false));
					else throw ("FileReader::ReadFile error in boolean value at line " + std::to_string(lineNumber));
				} 
				else if (subs[0] == string("int")) {
					parsedInt.insert(std::make_pair(subs[1], int(std::stof(temp))));
				} 
				else {
					throw ("FileReader::ReadFile unsupported data type " + subs[0] + " at line "+ std::to_string(lineNumber));
				}
			}
			lineNumber++;
		}
		file.close();
	}
}


void FileReader::init() {
	this->test();
}

void FileReader::test() {
	cout << "networkDebug " << parsedBool["networkDebug"] << endl;
	cout << "nodeInitLow " << parsedFloat["nodeInitLow"] << endl;
	cout << "actionDepth " << parsedFloat["actionDepth"] << endl;
	cout << "positionIterations " << parsedInt["positionIterations"] << endl;
	cout << "defaultStepSize " << parsedFloat["defaultStepSize"] << endl;
	cout << "discountFactor " << parsedFloat["discountFactor"] << endl;
	system("pause");
}
//Cut at ;
string FileReader::cut(string& str) {
	try {
		size_t pos = str.find(';');
		return str.substr(0, pos);
	}
	catch (int e) {
		throw ("FileReader::Cut Invalid line at " + std::to_string(lineNumber));
	}
}

//Split into vector at ' '
vector<string>& FileReader::split(string& str) {
	size_t pos = 0;
	string token;
	vector<string> substrings;
	while ((pos = str.find(string(" "))) != string::npos) {
		token = str.substr(0, pos);
		substrings.push_back(token);
		cout << token << endl;
		str.erase(0, pos + 1);
	}
	return substrings;
}