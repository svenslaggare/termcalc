#include "commandengine.h"
#include <cmath>

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
	};

	//Aliases
	mCommands["q"] = mCommands["exit"];
	mCommands["quit"] = mCommands["exit"];
}

std::string toBase(int base, int value) {
	std::string res;
	while (value > 0) {
		res += std::to_string(value % base);
		value /= base;
	}

	return std::string(res.rbegin(), res.rend());
}

bool CommandEngine::execute(std::string line) {
	if (line[0] == ':' && line.size() > 1) {
		std::string cmd = line.substr(1);

		if (mCommands.count(cmd) > 0) {
			return mCommands[cmd]({});
		} else {
			std::cout << "There exists no command called '" + cmd + "'.";
		}
	}

	if (line == "") {
		return false;
	}

	try {
		auto res = mEngine.eval(line, mEnv);

		//Display only different base if result is an integer
		double resInt;
		if (std::modf(res, &resInt) == 0.0) {
			switch (mPrintNumBase) {
			case NumberBase::BINARY:
				std::cout << "0b" << toBase(2, (int)res) << std::endl;
				break;
			case NumberBase::DECIMAL:
				std::cout << std::dec << res << std::endl;
				break;
			case NumberBase::HEXADECIMAL:
				std::cout << std::hex << "0x" << (int)res << std::endl;
				break;	
			}
		} else {
			std::cout << std::dec << res << std::endl;
		}

		mEnv.set("ans", res);
	} catch (std::runtime_error& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	return false;
}