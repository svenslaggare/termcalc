#include "environment.h"
#include <cmath>

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
	//Float scope
	mScopes.insert({ ResultValueType::FLOAT, EnvironmentScope({
		{ "pi", ResultValue(3.141592653589793238463) }
	}, {
		{ "sin", Function("sin", 1, [this](FnArgs args) {
			return ResultValue(sin(args.at(0).floatValue()));
		}, "Computes the sine of x.") },
		{ "cos", Function("cos", 1, [this](FnArgs args) {
			return ResultValue(cos(args.at(0).floatValue()));
		}, "Computes the cosine of x.") },
		{ "tan", Function("tan", 1, [this](FnArgs args) {
			return ResultValue(tan(args.at(0).floatValue()));
		}, "Computes the tangent of x.") },
		{ "sqrt", Function("sqrt", 1, [this](FnArgs args) {
			return ResultValue(sqrt(args.at(0).floatValue()));
		}, "Computes the square root of x.") },
		{ "asin", Function("asin", 1, [this](FnArgs args) {
			return ResultValue(asin(args.at(0).floatValue()));
		}, "Computes the inverse sine of x.") },
		{ "acos", Function("acos", 1, [this](FnArgs args) {
			return ResultValue(acos(args.at(0).floatValue()));
		}, "Computes the inverse cosine of x.") },
		{ "atan", Function("atan", 1, [this](FnArgs args) {
			return ResultValue(atan(args.at(0).floatValue()));
		}, "Computes the inverse tangent of x.") },
		{ "ln", Function("ln", 1, [this](FnArgs args) {
			return ResultValue(log(args.at(0).floatValue()));
		}, "Computes the natural logarithm of x.") },
		{ "log", Function("log", 1, [this](FnArgs args) {
			return ResultValue(log10(args.at(0).floatValue()));
		}, "Computes the 10-logarithm of x.") },
		{ "logb", Function("logb", 2, [this](FnArgs args) {
			return ResultValue(log(args.at(0).floatValue()) / log(args.at(1).floatValue()));
		}, "Computes the y-logarithm of x.") },
		{ "ceil", Function("ceil", 1, [this](FnArgs x) {
			return ResultValue(ceil(x.at(0).floatValue()));
		}, "Ceils x.") },
		{ "floor", Function("floor", 1, [this](FnArgs x) {
			return ResultValue(floor(x.at(0).floatValue()));
		}, "Floors x.") },
		{ "round", Function("round", 1, [this](FnArgs x) {
			return ResultValue(round(x.at(0).floatValue()));
		}, "Rounds x.") },
	}) });

	//Int scope
	mScopes.insert({ ResultValueType::INTEGER, EnvironmentScope(Variables({}), {
		{ "xor", Function("xor", 2, [this](FnArgs args) {
			return ResultValue(args.at(0).intValue() ^ args.at(1).intValue());
		}, "Bitwise XOR between x and y.") },
		{ "mod", Function("mod", 2, [this](FnArgs args) {
			std::int64_t result = args.at(0).intValue() % args.at(1).intValue();

			if (result < 0) {
				result += args.at(1).intValue();
			}

			return ResultValue(result);
		}, "Computes x mod y.") },
		{ "gcd", Function("gcd", 2, [this](FnArgs args) {
			auto x = args.at(0).intValue();
			auto y = args.at(1).intValue();

			std::int64_t reminder;

			while (y != 0) {
				reminder = x % y;
				x = y;
				y = reminder;
			}

			return ResultValue(x);
		}, "Computes the greatest common divisor between x and y.") },
	}) });
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