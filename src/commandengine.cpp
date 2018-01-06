#include "commandengine.h"
#include "core/numberhelpers.h"
#include "visitors/printvisitor.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream> 
#include <algorithm>

CommandEngine::CommandEngine(std::ostream& os)
	: mOutStream(os), mEngine(os), mEnv(mEngine.defaultEnvironment()) {
	std::string leadingWhitespace = "   ";

	mEngine.setEvalMode(ResultValueType::FLOAT);
	mEnv.setEvalMode(mEngine.evalMode());

	mCommands = {
		{ "help", [=](Args args) {
			mOutStream << "Commands:" << std::endl;
			mOutStream << leadingWhitespace << ":exit|:q|:quit     Exits the program." << std::endl;
			mOutStream << leadingWhitespace << ":mode              Sets the evaluation mode: float (default), int or complex." << std::endl;
			mOutStream << leadingWhitespace << ":display           Sets to display the result in the given base." << std::endl;
			mOutStream << leadingWhitespace << ":polar             Sets if complex numbers are printed in polar form." << std::endl;
			mOutStream << leadingWhitespace << ":vars              Prints the defined variables." << std::endl;
			mOutStream << leadingWhitespace << ":funcs             Prints the defined functions." << std::endl;
			return false;
		} },
		{ "exit", [](Args args) {
			return true;
		} },
		{ "display", [&](Args args) {
			if (args.size() == 1) {
				setPrintNumBase(std::stoi(args[0]));
			} else {
				mOutStream << "Expected one argument (int >= 2)." << std::endl;
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
					mOutStream << "'" << args[0] << "' is not a valid value. Valid values are: float, int and complex." << std::endl;
				}

				if (evalMode != ResultValueType::NONE) {
					setEvalMode(evalMode);
				}
			} else {
				mOutStream << "Expected one argument (float or int)." << std::endl;
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
					mOutStream << "Invalid value. Valid values are: true or false." << std::endl;
				}
			} else {
				mOutStream << "Expected one argument (true or false)." << std::endl;
			}

			return false;
		} },
		{ "vars", [&](Args args) {
			for (auto var : mEnv.variables()) {
				mOutStream << var.first << ": " << var.second << std::endl;
			}

			return false;
		} },
		{ "funcs", [this, leadingWhitespace](Args args) {
			//Compute the length of the longest function signature
			int maxFuncLength = 0;
			std::vector<std::string> funcStrs;

			for (auto& current : mEnv.functions()) {
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
			mOutStream << "Built-in:" << std::endl;

			for (auto& current : mEnv.functions()) {
				auto& func = current.second;

				if (!func.isUserDefined()) {
					auto funcStr = funcStrs[i];
					std::string spaceStr(maxFuncLength - funcStr.length(), ' ');
					mOutStream << leadingWhitespace << funcStr << spaceStr << func.infoText() << std::endl;
				} else {
					anyUserDefined = true;
				}

				i++;
			}

			//Check if any user defined functions
			if (anyUserDefined) {
				mOutStream << "User defined:" << std::endl;

				i = 0;
				for (auto& current : mEnv.functions()) {
					auto& func = current.second;

					if (func.isUserDefined()) {
						auto funcStr = funcStrs[i];
//						mOutStream << leadingWhitespace << funcStr << " = " << func.body()->toString() << std::endl;
						mOutStream << leadingWhitespace << funcStr << " = " << PrintVisitor::toString(mEngine, func.body()->body()) << std::endl;
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

void CommandEngine::setPrintNumBase(int base) {
	try {
		if (base >= 2 && base <= 36) {
			mPrintNumBase = base;
		} else {
			mOutStream << "The base must be >= 2 and <= 36." << std::endl;
		}
	} catch (std::exception& e) {
		mOutStream << "The base must be an integer." << std::endl;
	}
}

void CommandEngine::setEvalMode(ResultValueType evalMode) {
	mEngine.setEvalMode(evalMode);
	mEnv.setEvalMode(evalMode);
}

void CommandEngine::setPrintInPolar(bool printInPolar) {
	mPrintInPolar = printInPolar;
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
		mOutStream << "Could not open the file '" << fileName << "'." << std::endl;
	}
}

bool CommandEngine::execute(std::string line, bool printResult) {
	if (line.size() > 1 && line[0] == ':') {
		auto parts = splitString(line.substr(1), " ");
		std::string cmd = parts[0];

		if (mCommands.count(cmd) > 0) {
			parts.erase(parts.begin());
			return mCommands[cmd](parts);
		} else {
			mOutStream << "There exists no command called '" + cmd + "'. Type ':help' for a list of commands." << std::endl;
		}

		return false;
	}

	if (line.empty()) {
		return false;
	}

	try {
		auto res = mEngine.eval(line, mEnv);

		if (res.type() != ResultValueType::NONE) {
			if (printResult) {
				if (res.type() == ResultValueType::INTEGER) {
					switch (mPrintNumBase) {
						case 2:
							mOutStream << NumberHelpers::toBase(res.intValue(), 2, "0b") << std::endl;
							break;
						case 10:
							mOutStream << res.intValue() << std::endl;
							break;
						case 16:
							mOutStream << NumberHelpers::toBase(res.intValue(), 16, "0x") << std::endl;
							break;
						default:
							std::string baseSubscript = "_" + std::to_string(mPrintNumBase);

							#ifdef __unix__
							baseSubscript = getSubscript(mPrintNumBase);
							#endif

							mOutStream << NumberHelpers::toBase(res.intValue(), mPrintNumBase) << baseSubscript << std::endl;
							break;
					}
				} else if (res.type() == ResultValueType::COMPLEX && mPrintInPolar) {
					mOutStream << std::abs(res.complexValue()) << " * e^(" << std::arg(res.complexValue()) << "i)" << std::endl;
				} else {
					mOutStream << res << std::endl;
				}

				mEnv.set("ans", res);
			}
		}
	} catch (std::exception& e) {
		mOutStream << "Error: " << e.what() << std::endl;
	}

	return false;
}
