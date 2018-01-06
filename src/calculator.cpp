#include "calculator.h"
#include "calculation/numberhelpers.h"
#include "visitors/printvisitor.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream> 
#include <algorithm>

Calculator::Calculator(std::ostream& os)
	: mOutputStream(os), mEngine(os), mEnvironment(mEngine.defaultEnvironment()) {
	std::string leadingWhitespace = "   ";

	mEngine.setEvalMode(ResultValueType::FLOAT);
	mEnvironment.setEvalMode(mEngine.evalMode());

	mCommands = {
		{ "help", [=](Args args) {
			mOutputStream << "Commands:" << std::endl;
			mOutputStream << leadingWhitespace << ":exit|:q|:quit     Exits the program." << std::endl;
			mOutputStream << leadingWhitespace << ":mode              Sets the evaluation mode: float (default), int or complex." << std::endl;
			mOutputStream << leadingWhitespace << ":display           Sets to display the result in the given base." << std::endl;
			mOutputStream << leadingWhitespace << ":polar             Sets if complex numbers are printed in polar form." << std::endl;
			mOutputStream << leadingWhitespace << ":vars              Prints the defined variables." << std::endl;
			mOutputStream << leadingWhitespace << ":funcs             Prints the defined functions." << std::endl;
			return false;
		} },
		{ "exit", [](Args args) {
			return true;
		} },
		{ "display", [&](Args args) {
			if (args.size() == 1) {
				setPrintNumBase(std::stoi(args[0]));
			} else {
				mOutputStream << "Expected one argument (int >= 2)." << std::endl;
			}

			return false;
		} },
		{ "mode", [&](Args args) {
			if (args.size() == 1) {
				ResultValueType evalMode = ResultValueType::NONE;

				if (args[0] == "float") {
					evalMode = ResultValueType::FLOAT;
				} else if (args[0] == "int") {
					evalMode = ResultValueType::INTEGER;
				} else if (args[0] == "complex") {
					evalMode = ResultValueType::COMPLEX;
				} else {
					mOutputStream << "'" << args[0] << "' is not a valid value. Valid values are: float, int and complex." << std::endl;
				}

				if (evalMode != ResultValueType::NONE) {
					setEvalMode(evalMode);
				}
			} else {
				mOutputStream << "Expected one argument (float or int)." << std::endl;
			}

			return false;
		} },
		{ "polar", [&](Args args) {
			if (args.size() == 1) {
				if (args[0] == "true" || args[0] == "1") {
					setPrintInPolar(true);
				} else if (args[0] == "false" || args[0] == "0") {
					setPrintInPolar(false);
				} else {
					mOutputStream << "Invalid value. Valid values are: true or false." << std::endl;
				}
			} else {
				mOutputStream << "Expected one argument (true or false)." << std::endl;
			}

			return false;
		} },
		{ "vars", [&](Args args) {
			for (auto var : mEnvironment.variables()) {
				mOutputStream << var.first << ": " << var.second << std::endl;
			}

			return false;
		} },
		{ "funcs", [this, leadingWhitespace](Args args) {
			//Compute the length of the longest function signature
			int maxFuncLength = 0;
			std::vector<std::string> funcStrs;

			for (auto& current : mEnvironment.functions()) {
				std::stringstream stream;
				auto& func = current.second;
				stream << func;
				funcStrs.push_back(stream.str());

				stream.seekg(0, std::ios::end);
				maxFuncLength = std::max(maxFuncLength, (int)stream.tellg());
			}	

			maxFuncLength += 3;

			std::size_t i = 0;
			bool anyUserDefined = false;
			mOutputStream << "Built-in:" << std::endl;

			for (auto& current : mEnvironment.functions()) {
				auto& func = current.second;

				if (!func.isUserDefined()) {
					auto funcStr = funcStrs[i];
					std::string spaceStr(maxFuncLength - funcStr.length(), ' ');
					mOutputStream << leadingWhitespace << funcStr << spaceStr << func.infoText() << std::endl;
				} else {
					anyUserDefined = true;
				}

				i++;
			}

			//Check if any user defined functions
			if (anyUserDefined) {
				mOutputStream << "User defined:" << std::endl;

				i = 0;
				for (auto& current : mEnvironment.functions()) {
					auto& func = current.second;

					if (func.isUserDefined()) {
						auto funcStr = funcStrs[i];
//						mOutStream << leadingWhitespace << funcStr << " = " << func.body()->toString() << std::endl;
						mOutputStream << leadingWhitespace << funcStr << " = " << PrintVisitor::toString(mEngine,
																									  func.userFunction()->body()) << std::endl;
					}

					i++;
				}
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
	const std::vector<std::string> subscripts {
		"\xe2\x82\x80",
        "\xe2\x82\x81",
        "\xe2\x82\x82",
		"\xe2\x82\x83",
        "\xe2\x82\x84",
        "\xe2\x82\x85",
		"\xe2\x82\x86",
        "\xe2\x82\x87",
        "\xe2\x82\x88",
        "\xe2\x82\x89"
	};

	//Returns the given number as a subscript
	std::string getSubscript(std::int64_t num) {
		std::string str = std::to_string(num);
		std::string subscriptStr;

        for (char c : str) {
            subscriptStr += subscripts[c - '0'];
        }

		return subscriptStr;
	}

	//Splits the given string
	std::vector<std::string> splitString(std::string str, const std::string& delimiter) {
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

void Calculator::setPrintNumBase(int base) {
	try {
		if (base >= 2 && base <= 36) {
			mPrintNumBase = base;
		} else {
			mOutputStream << "The base must be >= 2 and <= 36." << std::endl;
		}
	} catch (std::exception& e) {
		mOutputStream << "The base must be an integer." << std::endl;
	}
}

void Calculator::setEvalMode(ResultValueType evalMode) {
	mEngine.setEvalMode(evalMode);
	mEnvironment.setEvalMode(evalMode);
}

void Calculator::setPrintInPolar(bool printInPolar) {
	mPrintInPolar = printInPolar;
}

void Calculator::loadFile(const std::string& fileName, bool printIfNotFound) {
	std::ifstream stream(fileName);

	if (stream.is_open()) {
		std::string line;

		while (!stream.eof()) {
			std::getline(stream, line);
			execute(line, false);
		}
	} else if (printIfNotFound) {
		mOutputStream << "Could not open the file '" << fileName << "'." << std::endl;
	}
}

bool Calculator::execute(const std::string& line, bool printResult) {
	if (line.size() > 1 && line[0] == ':') {
		auto parts = splitString(line.substr(1), " ");
		std::string cmd = parts[0];

		if (mCommands.count(cmd) > 0) {
			parts.erase(parts.begin());
			return mCommands[cmd](parts);
		} else {
			mOutputStream << "There exists no command called '" + cmd + "'. Type ':help' for a list of commands." << std::endl;
		}

		return false;
	}

	if (line.empty()) {
		return false;
	}

	try {
		auto res = mEngine.evaluate(line, mEnvironment);

		if (res.type() != ResultValueType::NONE) {
			if (printResult) {
				if (res.type() == ResultValueType::INTEGER) {
					switch (mPrintNumBase) {
						case 2:
							mOutputStream << NumberHelpers::toBase(res.intValue(), 2, "0b") << std::endl;
							break;
						case 10:
							mOutputStream << res.intValue() << std::endl;
							break;
						case 16:
							mOutputStream << NumberHelpers::toBase(res.intValue(), 16, "0x") << std::endl;
							break;
						default:
							std::string baseSubscript = "_" + std::to_string(mPrintNumBase);

							#ifdef __unix__
							baseSubscript = getSubscript(mPrintNumBase);
							#endif

							mOutputStream << NumberHelpers::toBase(res.intValue(), mPrintNumBase) << baseSubscript << std::endl;
							break;
					}
				} else if (res.type() == ResultValueType::COMPLEX && mPrintInPolar) {
					mOutputStream << std::abs(res.complexValue()) << " * e^(" << std::arg(res.complexValue()) << "i)" << std::endl;
				} else {
					mOutputStream << res << std::endl;
				}

				mEnvironment.set("ans", res);
			}
		}
	} catch (std::exception& e) {
		mOutputStream << "Error: " << e.what() << std::endl;
	}

	return false;
}
