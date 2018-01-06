#include "function.h"
#include "../expressions/expression.h"
#include "environment.h"
#include <unordered_map>

//Function body
UserFunction::UserFunction(std::vector<std::string> parameters, std::unique_ptr<Expression> body)
	: mParameters(parameters), mBody(std::move(body)) {

}

const std::vector<std::string>& UserFunction::parameters() const {
	return mParameters;
}

Expression* UserFunction::body() const {
	return mBody.get();
}

//Function
Function::Function(std::string name, std::size_t numParameters, ExternalFunction func, std::string infoText)
	: mName(name), mNumArgs(numParameters), mIsUserDefined(false), mExternalFunction(func), mInfoText(infoText) {

}

Function::Function(std::string name, std::size_t numParameters, std::shared_ptr<UserFunction> body, std::string infoText)
	: mName(name), mNumArgs(numParameters), mIsUserDefined(true), mUserFunction(body), mInfoText(infoText) {

}

std::string Function::name() const {
	return mName;
}

std::size_t Function::numParameters() const {
	return mNumArgs;
}

bool Function::isUserDefined() const {
	return mIsUserDefined;
}

std::shared_ptr<UserFunction> Function::userFunction() const {
	return mUserFunction;
}

ExternalFunction Function::externalFunction() const {
	return mExternalFunction;
}

std::string Function::infoText() const {
	return mInfoText;
}

namespace {
	const std::unordered_map<int, std::string> argNameTable = {
		{ 0, "x" },
		{ 1, "y" },
		{ 2, "z" },
		{ 3, "w" },
	};
}

std::ostream& operator<<(std::ostream& os, const Function& func) {
	os << func.name();

	os << "(";

	if (!func.isUserDefined()) {
		for (std::size_t i = 0; i < func.numParameters(); i++) {
			if (i != 0) {
				os << ", ";
			}

			if (argNameTable.count(i) > 0) {
				os << argNameTable.at(i);
			} else {
				os << "x" << i;
			}
		}
	} else {
		bool isFirst = true;
		for (auto param : func.userFunction()->parameters()) {
			if (!isFirst) {
				os << ", ";
			}

			os << param;

			isFirst = false;
		}
	}

	os << ")";

	return os;
}