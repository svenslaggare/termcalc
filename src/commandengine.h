#pragma once
#include "calcengine.h"
#include "environment.h"
#include <unordered_map>
#include <functional>
#include <vector>

using Args = std::vector<std::string>;

//Represents a command engine
class CommandEngine {
private:
	std::ostream& mOutStream;
	CalcEngine mEngine;
	Environment mEnv;
	int mPrintNumBase = 10;
	bool mPrintInPolar = false;
	std::unordered_map<std::string, std::function<bool(Args)>> mCommands;
public:
	//Creates a new command engine
	CommandEngine(std::ostream& os = std::cout);

	//Sets the print num base
	void setPrintNumBase(int base);

	//Sets the evaluation mode
	void setEvalMode(ResultValueType evalMode);

	//Sets if complex numbers are printed in polar form
	void setPrintInPolar(bool printInPolar);

	//Loads the given file
	void loadFile(std::string fileName, bool printIfNotFound = true);

	//Executes the given line
	bool execute(std::string line, bool printResult = true);
};