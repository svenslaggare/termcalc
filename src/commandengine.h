#pragma once
#include "calcengine.h"
#include <unordered_map>
#include <functional>
#include <vector>

//The number bases
enum class NumberBase {
	BINARY,
	DECIMAL,
	HEXADECIMAL
};

using Args = std::vector<std::string>;

//Represents a command engine
class CommandEngine {
private:
	Environment mEnv;
	CalcEngine mEngine;
	NumberBase mPrintNumBase = NumberBase::DECIMAL;
	std::unordered_map<std::string, std::function<bool(Args)>> mCommands;
public:
	//Creates a new command engine
	CommandEngine();

	//Executes the given line
	bool execute(std::string line);
};