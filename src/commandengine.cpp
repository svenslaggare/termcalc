#include "commandengine.h"
#include <cmath>
#include <iostream>

CommandEngine::CommandEngine() {
	mCommands = {
		{ "help", [](Args args) {
			std::string cmdStart = "   ";
			std::cout << "Commands:" << std::endl;
			std::cout << cmdStart << ":exit|:q|:quit Exits the program." << std::endl;
			std::cout << cmdStart << ":bin           Sets to display the result in base 2." << std::endl;
			std::cout << cmdStart << ":dec           Sets to display the result in base 10. (default)" << std::endl;
			std::cout << cmdStart << ":hex           Sets to display the result in base 16." << std::endl;
			std::cout << cmdStart << ":vars          Prints the defined variables." << std::endl;
			std::cout << cmdStart << ":mode          Sets the evaluation mode: float (default) or int." << std::endl;
			return false;
		} },
		{ "exit", [](Args args) {
			return true;
		} },
		{ "bin", [&](Args args) {
			mPrintNumBase = NumberBase::BINARY;
			return false;
		} },
		{ "dec", [&](Args args) {
			mPrintNumBase = NumberBase::DECIMAL;
			return false;
		} },
		{ "hex", [&](Args args) {
			mPrintNumBase = NumberBase::HEXADECIMAL;
			return false;
		} },
		{ "vars", [&](Args args) {
			for (auto var : mEnv.variables()) {
				std::cout << var.first << ": " << var.second << std::endl;
			}
			return false;
		} },
		{ "mode", [&](Args args) {
			if (args.size() == 1) {
				if (args[0] == "float") {
					mEngine.setEvalMode(ResultValueType::FLOAT);
				} else if (args[0] == "int") {
					mEngine.setEvalMode(ResultValueType::INTEGER);
				} else {
					std::cout << "'" << args[0] << "' is not a valid value. Valid values are: float and int." << std::endl;
				}
			} else {
				std::cout << "Expected one argument (float or int)." << std::endl;
			}
			return false;
		} },
	};

	//Aliases
	mCommands["q"] = mCommands["exit"];
	mCommands["quit"] = mCommands["exit"];
	mCommands["h"] = mCommands["help"];
}

std::string toBase(int base, long value) {
	std::string res;
	while (value > 0) {
		res += std::to_string(value % base);
		value /= base;
	}

	return std::string(res.rbegin(), res.rend());
}

std::vector<std::string> splitString(std::string str, std::string delimiter) {
	std::vector<std::string> parts;

	size_t pos = 0;
	std::string token;
	while ((pos = str.find(delimiter)) != std::string::npos) {
	    token = str.substr(0, pos);
	    parts.push_back(token);
	    str.erase(0, pos + delimiter.length());
	}
	
	parts.push_back(str);
	return parts;
}

bool CommandEngine::execute(std::string line) {
	if (line[0] == ':' && line.size() > 1) {
		auto parts = splitString(line.substr(1), " ");
		std::string cmd = parts[0];

		if (mCommands.count(cmd) > 0) {
			parts.erase(parts.begin());
			return mCommands[cmd](parts);
		} else {
			std::cout << "There exists no command called '" + cmd + "'. Type ':help' for a list of commands." << std::endl;
		}

		return false;
	}

	if (line == "") {
		return false;
	}

	try {
		auto res = mEngine.eval(line, mEnv);

		if (res.type() == ResultValueType::FLOAT) {
			//Display only different base if result is an integer
			double resInt;
			if (std::modf(res.doubleValue(), &resInt) == 0.0) {
				switch (mPrintNumBase) {
				case NumberBase::BINARY:
					std::cout << "0b" << toBase(2, (long)res.doubleValue()) << std::endl;
					break;
				case NumberBase::DECIMAL:
					std::cout << std::dec << res << std::endl;
					break;
				case NumberBase::HEXADECIMAL:
					std::cout << std::hex << "0x" << (long)res.doubleValue() << std::endl;
					break;	
				}
			} else {
				std::cout << std::dec << res << std::endl;
			}
		} else {
			switch (mPrintNumBase) {
			case NumberBase::BINARY:
				std::cout << "0b" << toBase(2, res.longValue()) << std::endl;
				break;
			case NumberBase::DECIMAL:
				std::cout << std::dec << res << std::endl;
				break;
			case NumberBase::HEXADECIMAL:
				std::cout << std::hex << "0x" << res.longValue() << std::endl;
				break;	
			}
		}

		mEnv.set("ans", res);
	} catch (std::runtime_error& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	return false;
}