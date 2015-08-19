#include "environment.h"
#include <cmath>

Environment::Environment() {
	mValues = {
		{ "pi", ResultValue(3.141592653589793238463) }
	};

	mFunctions = {
		{ "sin", Function("sin", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, sin(x.at(0).floatValue()));
		}, "Computes the sine of x.") },
		{ "cos", Function("cos", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, cos(x.at(0).floatValue()));
		}, "Computes the cosine of x.") },
		{ "tan", Function("tan", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, tan(x.at(0).floatValue()));
		}, "Computes the tangent of x.") },
		{ "sqrt", Function("sqrt", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, sqrt(x.at(0).floatValue()));
		}, "Computes the square root of x.") },
		{ "asin", Function("asin", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, asin(x.at(0).floatValue()));
		}, "Computes the inverse sine of x.") },
		{ "acos", Function("acos", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, acos(x.at(0).floatValue()));
		}, "Computes the inverse cosine of x.") },
		{ "atan", Function("atan", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, atan(x.at(0).floatValue()));
		}, "Computes the inverse tangent of x.") },
		{ "ln", Function("ln", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, log(x.at(0).floatValue()));
		}, "Computes the natrual logaritm of x.") },
		{ "log", Function("log", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, log10(x.at(0).floatValue()));
		}, "Computes the 10-logaritm of x.") },
		{ "logb", Function("logb", 2, [this](FnArgs x) {
			return ResultValue(mEvalMode, log(x.at(0).floatValue()) / log(x.at(1).floatValue()));
		}, "Computes the y-logaritm of x.") },
		{ "xor", Function("xor", 2, [this](FnArgs x) {
			return ResultValue(mEvalMode, x.at(0).intValue() ^ x.at(1).intValue());
		}, "Bitwise XOR between x and y.") },
		{ "mod", Function("mod", 2, [this](FnArgs x) {
			std::int64_t result = x.at(0).intValue() % x.at(1).intValue();

			if (result < 0) {
				result += x.at(1).intValue();
			}

			return ResultValue(mEvalMode, result);
		}, "Computes x mod y.") },
	};
}

const std::unordered_map<std::string, ResultValue>& Environment::variables() {
	return mValues;
}

void Environment::set(std::string variable, ResultValue value) {
	mValues[variable] = value;
}

bool Environment::getVariable(std::string variable, ResultValue& value, bool applyConversion) const {
	if (mValues.count(variable) > 0) {
		if (applyConversion) {
			value = mValues.at(variable).convertTo(mEvalMode);
		} else {
			value = mValues.at(variable);
		}

		return true;
	} else {
		return false;
	}
}

void Environment::unset(std::string variable) {
	mValues.erase(variable);
}

ResultValue Environment::valueOf(std::string variable) const {
	return mValues.at(variable);
}

const std::unordered_map<std::string, Function>& Environment::functions() const {
	return mFunctions;
}

void Environment::define(Function function) {
	if (mFunctions.count(function.name()) == 0) {
		mFunctions.insert({ function.name(), function });
	} else {
		mFunctions.at(function.name()) = function;
	}
}

ResultValueType Environment::evalMode() const {
	return mEvalMode;
}

void Environment::setEvalMode(ResultValueType evalMode) {
	mEvalMode = evalMode;
}