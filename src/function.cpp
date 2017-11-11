#include "function.h"
#include "expression.h"
#include "environment.h"
#include <unordered_map>

//Function body
FunctionBody::FunctionBody(std::vector<std::string> parameters, std::unique_ptr<Expression> body)
	: mParameters(parameters), mBody(std::move(body)) {

}

const std::vector<std::string>& FunctionBody::parameters() const {
	return mParameters;
}

std::string FunctionBody::toString() const {
	return mBody->toString();
}

ResultValue FunctionBody::apply(CalcEngine& calcEngine, Environment& environment, FnArgs args) const {
	//First, save the environment values
	std::unordered_map<std::string, ResultValue> savedValues;
	for (auto param : mParameters) {
		if (environment.variables().count(param) > 0) {
			savedValues.insert({ param, environment.variables().at(param) });
		}
	}

	//Set the parameter values
	std::size_t i = 0;
	for (auto param : mParameters) {
		environment.set(param, args[i]);
		i++;
	}

	//Apply the function
	EvalStack stack;
	mBody->evaluate(calcEngine, environment, stack);

	if (stack.size() != 1) {
		throw std::runtime_error("Expected result.");
	}

	//Restore environment values
	for (auto param : mParameters) {
		environment.unset(param);

		if (savedValues.count(param) > 0) {
			environment.set(param, savedValues[param]);
		}
	}

	return stack.top();
}

//Function
Function::Function(std::string name, std::size_t numArgs, ApplyFunction func, std::string infoText)
	: mName(name), mNumArgs(numArgs), mIsUserDefined(false), mFunc(func), mInfoText(infoText) {

}

Function::Function(std::string name, std::size_t numArgs, std::shared_ptr<FunctionBody> body, std::string infoText)
	: mName(name), mNumArgs(numArgs), mIsUserDefined(true), mUserBody(body), mInfoText(infoText) {

}

std::string Function::name() const {
	return mName;
}

std::size_t Function::numArgs() const {
	return mNumArgs;
}

bool Function::isUserDefined() const {
	return mIsUserDefined;
}

std::shared_ptr<FunctionBody> Function::body() const {
	return mUserBody;;
}

ResultValue Function::apply(CalcEngine& calcEngine, Environment& environment, FnArgs args) const {
	if (!mIsUserDefined) {
		return mFunc(args);
	} else {
		return mUserBody->apply(calcEngine, environment, args);
	}
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
		for (int i = 0; i < func.numArgs(); i++) {
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
		for (auto param : func.body()->parameters()) {
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