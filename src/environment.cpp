#include "environment.h"
#include <cmath>

EnvironmentScope::EnvironmentScope() {

}

EnvironmentScope::EnvironmentScope(Variables variables, std::unordered_map<std::string, Function> functions)
	: mVariables(variables), mFunctions(functions) {

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
	for (auto& current : mScopes) {
		auto& scope = current.second;
		if (scope.functions().count(function.name()) == 0) {
			scope.functions().insert({ function.name(), function });
		} else {
			scope.functions().at(function.name()) = function;
		}
	}
}

ResultValueType Environment::evalMode() const {
	return mEvalMode;
}

void Environment::setEvalMode(ResultValueType evalMode) {
	mEvalMode = evalMode;
}