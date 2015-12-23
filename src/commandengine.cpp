#include "commandengine.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream> 

CommandEngine::CommandEngine()
	: mEnv(mEngine.defaultEnvironment()) {
	std::string leadingWhitespace = "   ";

	mCommands = {
		{ "help", [=](Args args) {
			std::cout << "Commands:" << std::endl;
			std::cout << leadingWhitespace << ":exit|:q|:quit Exits the program." << std::endl;
			std::cout << leadingWhitespace << ":mode          Sets the evaluation mode: float (default), int or complex." << std::endl;
			std::cout << leadingWhitespace << ":display       Sets to display the result in the given base." << std::endl;
			std::cout << leadingWhitespace << ":vars          Prints the defined variables." << std::endl;
			std::cout << leadingWhitespace << ":funcs         Prints the defined functions." << std::endl;
			return false;
		} },
		{ "exit", [](Args args) {
			return true;
		} },
		{ "display", [&](Args args) {
			if (args.size() == 1) {
				try {
					auto base = std::stoi(args[0]);
					if (base >= 2) {
						mPrintNumBase = base;
					} else {
						std::cout << "The base must be >= 2." << std::endl;
					}
				} catch (std::exception& e) {
					std::cout << "The base must be an integer." << std::endl;
				}
			} else {
				std::cout << "Expected one argument (int >= 2)." << std::endl;
			}
			return false;
		} },
		{ "vars", [&](Args args) {
			for (auto var : mEnv.variables()) {
				std::cout << var.first << ": " << var.second << std::endl;
			}
			return false;
		} },
		{ "funcs", [this, leadingWhitespace](Args args) {
			//Compute the length of the longest function signature
			int maxFuncLength = 0;
			std::vector<std::string> funcStrs;

			for (auto& current : mEnv.functions()) {
				std::stringstream strstream;
				auto& func = current.second;
				strstream << func;
				funcStrs.push_back(strstream.str());

				strstream.seekg(0, std::ios::end);
				maxFuncLength = std::max(maxFuncLength, (int)strstream.tellg());
			}	

			maxFuncLength += 3;

			std::size_t i = 0;
			bool anyUserDefined = false;
			std::cout << "Builtin:" << std::endl;

			for (auto& current : mEnv.functions()) {
				auto& func = current.second;

				if (!func.isUserDefined()) {
					auto funcStr = funcStrs[i];
					std::string spaceStr(maxFuncLength - funcStr.length(), ' ');
					std::cout << leadingWhitespace << funcStr << spaceStr << func.infoText() << std::endl;
				} else {
					anyUserDefined = true;
				}

				i++;
			}

			//Check if any user defined functions
			if (anyUserDefined) {
				std::cout << "User defined:" << std::endl;

				i = 0;
				for (auto& current : mEnv.functions()) {
					auto& func = current.second;

					if (func.isUserDefined()) {
						auto funcStr = funcStrs[i];
						std::cout << leadingWhitespace << funcStr << " = " << func.body()->toString() << std::endl;
					}

					i++;
				}
			}

			return false;
		} },
		{ "mode", [&](Args args) {
			if (args.size() == 1) {
				if (args[0] == "float") {
					mEngine.setEvalMode(ResultValueType::FLOAT);
					mEnv.setEvalMode(ResultValueType::FLOAT);
				} else if (args[0] == "int") {
					mEngine.setEvalMode(ResultValueType::INTEGER);
					mEnv.setEvalMode(ResultValueType::INTEGER);
				} else if (args[0] == "complex") {
					mEngine.setEvalMode(ResultValueType::COMPLEX);
					mEnv.setEvalMode(ResultValueType::COMPLEX);
				} else {
					std::cout << "'" << args[0] << "' is not a valid value. Valid values are: float, int and complex." << std::endl;
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

namespace {
	//Converts the given value into a string with the given base
	std::string toBase(std::int64_t value, int base) {
		std::string res;
		while (value > 0) {
			res += std::to_string(value % base);
			value /= base;
		}

		return std::string(res.rbegin(), res.rend());
	}

	//Returns the given number as a subscript
	std::string getSubscript(int num) {
		static const std::vector<std::string> subscripts{ "\xe2\x82\x80", "\xe2\x82\x81", "\xe2\x82\x82",
														  "\xe2\x82\x83", "\xe2\x82\x84", "\xe2\x82\x85", "\xe2\x82\x86",
														  "\xe2\x82\x87", "\xe2\x82\x88", "\xe2\x82\x89"};

		std::string str = std::to_string(num);
		std::string substr;

		for (std::size_t i = 0; i < str.length(); i++) {
			substr += subscripts[str[i] - '0'];
		}

		return substr;
	}

	//Splits the given string
	std::vector<std::string> splitString(std::string str, std::string delimiter) {
		std::vector<std::string> parts;

		std::size_t pos = 0;
		std::string token;
		while ((pos = str.find(delimiter)) != std::string::npos) {
			token = str.substr(0, pos);
			parts.push_back(token);
			str.erase(0, pos + delimiter.length());
		}

		parts.push_back(str);
		return parts;
	}
}

void CommandEngine::loadFile(std::string fileName, bool printIfNotFound) {
	std::ifstream stream(fileName);

	if (stream.is_open()) {
		std::string line;

		while (!stream.eof()) {
			std::getline(stream, line);
			execute(line, false);
		}
	} else if (printIfNotFound) {
		std::cout << "Could not open the file '" << fileName << "'." << std::endl;
	}
}

bool CommandEngine::execute(std::string line, bool printResult) {
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

		if (printResult) {
			if (res.type() == ResultValueType::INTEGER) {
				switch (mPrintNumBase) {
					case 2:
						std::cout << "0b" << toBase(res.intValue(), 2) << std::endl;
						break;
					case 10:
						std::cout << res.intValue() << std::endl;
						break;
					case 16:
						std::cout << "0x" << toBase(res.intValue(), 16) << std::endl;
						break;
					default:
						std::string baseSubscript = "";

						#ifdef __unix__
						baseSubscript = getSubscript(mPrintNumBase);
						#endif

						std::cout << toBase(res.intValue(), mPrintNumBase) << baseSubscript << std::endl;
						break;
				}
			} else {
				std::cout << res << std::endl;
			}

			mEnv.set("ans", res);
		}
	} catch (std::runtime_error& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	return false;
}