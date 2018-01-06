#pragma once
#include "calculation/calcengine.h"
#include "calculation/environment.h"
#include <unordered_map>
#include <functional>
#include <vector>

using Args = std::vector<std::string>;

/**
 * Represents the calculator
 */
class Calculator {
private:
	std::ostream& mOutStream;
	CalculationEngine mEngine;
	Environment mEnv;
	int mPrintNumBase = 10;
	bool mPrintInPolar = false;
	std::unordered_map<std::string, std::function<bool(Args)>> mCommands;
public:
	//Creates a new command engine
	Calculator(std::ostream& os = std::cout);

	//Sets the print num base
	void setPrintNumBase(int base);

	//Sets the evaluation mode
	void setEvalMode(ResultValueType evalMode);

	//Sets if complex numbers are printed in polar form
	void setPrintInPolar(bool printInPolar);

	//Loads the given file
	void loadFile(const std::string& fileName, bool printIfNotFound = true);

	//Executes the given line
	bool execute(const std::string& line, bool printResult = true);
};