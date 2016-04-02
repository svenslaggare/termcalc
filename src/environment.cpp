#include "environment.h"
#include <cmath>

namespace {
	//Returns the name of the given function
	std::string getFuncName(std::string name, std::size_t numArgs) {
		std::stringstream stream;

		stream << name << "(";
		for (int i = 0; i < numArgs; i++) {
			if (i != 0) {
				stream << ", ";
			}

			stream << "x" << i;
		}

		stream << ")";

		return stream.str();
	}
}

EnvironmentScope::EnvironmentScope() {

}

EnvironmentScope::EnvironmentScope(Variables variables, std::vector<Function> functions)
	: mVariables(variables) {
	for (auto& func : functions) {
		mFunctions.insert({ getFuncName(func.name(), func.numArgs()), func });
	}
}

Variables& EnvironmentScope::variables() {
	return mVariables;
}

const Variables& EnvironmentScope::variables() const {
	return mVariables;
}

std::unordered_map<std::string, Function>& EnvironmentScope::functions() {
	return mFunctions;
}

const std::unordered_map<std::string, Function>& EnvironmentScope::functions() const {
	return mFunctions;
}

Environment::Environment() {
	for (auto scope : ResultValueTypes::all) {
		mScopes.insert({ scope, EnvironmentScope() });
	}
}

Environment::Environment(std::unordered_map<ResultValueType, EnvironmentScope> scopes)
	 : mScopes(scopes) {

}

EnvironmentScope& Environment::currentScope() {
	return mScopes.at(mEvalMode);
}

const EnvironmentScope& Environment::currentScope() const {
	return mScopes.at(mEvalMode);
}

const Variables& Environment::variables() const {
	return currentScope().variables();
}

void Environment::set(std::string variable, ResultValue value) {
	for (auto& current : mScopes) {
		auto& scope = current.second;
		scope.variables()[variable] = value;
	}
}

bool Environment::getVariable(std::string variable, ResultValue& value, bool applyConversion) const {
	if (variables().count(variable) > 0) {
		if (applyConversion) {
			value = variables().at(variable).convertTo(mEvalMode);
		} else {
			value = variables().at(variable);
		}

		return true;
	} else {
		return false;
	}
}

void Environment::unset(std::string variable) {
	for (auto& current : mScopes) {
		auto& scope = current.second;
		scope.variables().erase(variable);
	}
}

ResultValue Environment::valueOf(std::string variable) const {
	return variables().at(variable);
}

const Functions& Environment::functions() const {
	return currentScope().functions();
}

void Environment::define(Function function) {
	auto funcName = getFuncName(function.name(), (int)function.numArgs());

	for (auto& current : mScopes) {
		auto& scope = current.second;
		if (scope.functions().count(funcName) == 0) {
			scope.functions().insert({ funcName, function });
		} else {
			scope.functions().at(funcName) = function;
		}
	}
}

const Function& Environment::getFunction(std::string name, std::size_t numArgs) {
	std::string funcName = getFuncName(name, numArgs);

	if (functions().count(funcName) == 0) {
		throw std::runtime_error("'" + funcName + "' is not a defined function.");
	}

	return functions().at(funcName);
}

ResultValueType Environment::evalMode() const {
	return mEvalMode;
}

void Environment::setEvalMode(ResultValueType evalMode) {
	mEvalMode = evalMode;
}