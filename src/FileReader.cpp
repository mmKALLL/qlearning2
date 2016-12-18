#include "FileReader.hpp"


FileReader::FileReader()
{
	readFile(filename);
}

void FileReader::readFile(string name) {
	std::ifstream file(name);
	
	if (!file.is_open()) { // The file could not be opened
		throw "FileReader: File could not be opened";
	}
	else { // File can be safely used
		string line;
		while (std::getline(file, line)) {
			cout << lineNumber << ": " << line << endl;
			vector<string> subs = split(line);
			if (subs.size() > 3 &&
				subs[0].at(0) != '/' && 
				subs[2].at(0) == '=' &&
				subs[0].at(0) != '\n') 
											{
				//Line is valid
				//Form is typename variablename = value
				string temp = subs[3];
				//Parse! :D
				if (subs[0] == string("float")) {
					parsedFloat[subs[1]] = std::stof(temp);
				}
				else if (subs[0] == string("bool")) {
					if (temp == string("true")) parsedBool[subs[1]] = true;
					else if (temp == string("false")) parsedBool[subs[1]] = false;
					else throw ("FileReader::ReadFile error in boolean spelling at line " + lineNumber);
				} 
				else if (subs[0] == string("int")) {
					parsedInt.insert(std::make_pair(subs[1], std::stoi(temp)));
				} 
				else {
					throw ("FileReader::ReadFile unsupported data type " + subs[0] + " at line " + std::to_string(lineNumber));
				}
			}
			lineNumber++;
		}
		file.close();
	}
}


void FileReader::init() {
	readFile(filename);
	//this->test();
}

void FileReader::test() {
	cout << "networkDebug " << parsedBool[string("networkDebug")] << endl;
	cout << "nodeInitLow " << parsedFloat.at("nodeInitLow") << endl;
	cout << "actionDepth " << parsedFloat["actionDepth"] << endl;
	cout << "positionIterations " << parsedInt["positionIterations"] << endl;
	cout << "defaultStepSize " << parsedFloat["defaultStepSize"] << endl;
	cout << "discountFactor " << parsedFloat["discountFactor"] << endl;

	cout << endl << "Printing floats: " << endl;
	for (auto it = parsedFloat.begin(); it != parsedFloat.end(); it++) {
		cout << it->first << " " << it->second << endl;
	}
	cout << endl << "Printing bools: " << endl;
	for (auto it = parsedBool.begin(); it != parsedBool.end(); it++) {
		cout << it->first << " " << it->second << endl;
	}
	cout << endl << "Printing ints: " << endl;
	for (auto it = parsedInt.begin(); it != parsedInt.end(); it++) {
		cout << it->first << " " << it->second << endl;
	}

	system("pause");
}
//Cut at ;
string FileReader::cut(string& str) {
	try {
		size_t pos = str.find(';');
		return (str.substr(0, pos) + string(" "));
	}
	catch (int e) {
		throw ("FileReader::Cut Invalid line at " + std::to_string(lineNumber));
	}
}

//Split into vector at ' '
vector<string> FileReader::split(string& uncut) {
	//if (str.size() < 2) return vector<string> {"/"};
	size_t pos = 0;
	string token;
	vector<string> substrings;
	string str = cut(uncut);
	while ((pos = str.find(string(" "))) != string::npos) {
		token = str.substr(0, pos);
		substrings.push_back(token);
		str.erase(0, pos + 1);
	}
	return substrings;
}