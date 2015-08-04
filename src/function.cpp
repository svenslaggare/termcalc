#include "function.h"
#include <unordered_map>

Function::Function(std::string name, std::size_t numArgs, ApplyFunction func, std::string infoText)
	: mName(name), mNumArgs(numArgs), mFunc(func), mInfoText(infoText) {

}

std::string Function::name() const {
	return mName;
}

std::size_t Function::numArgs() const {
	return mNumArgs;
}

ResultValue Function::apply(FnArgs args) const {
	return mFunc(args);
}

std::string Function::infoText() const {
	return mInfoText;
}

namespace {
	std::unordered_map<int, std::string> argNameTable = {
		{ 0, "x" },
		{ 1, "y" },
		{ 2, "z" },
		{ 3, "w" },
	};
}

std::ostream& operator<<(std::ostream& os, const Function& func) {
	os << func.name();

	os << "(";

	for (int i = 0; i < func.numArgs(); i++) {
		if (i != 0) {
			os << ", ";
		}

		if (argNameTable.count(i) > 0) {
			os << argNameTable[i];
		} else {
			os << "x" << i;
		}
	}

	os << ")";

	return os;
}