#include "environment.h"
#include <cmath>

Environment::Environment() {
	mValues = {
		{ "pi", ResultValue(3.141592653589793238463) }
	};

	mFunctions = {
		{ "sin", Function("sin", 1, [this](FnArgs args) {
			return ResultValue(mEvalMode, sin(args.at(0).floatValue()));
		}, "Computes the sine of x.") },
		{ "cos", Function("cos", 1, [this](FnArgs args) {
			return ResultValue(mEvalMode, cos(args.at(0).floatValue()));
		}, "Computes the cosine of x.") },
		{ "tan", Function("tan", 1, [this](FnArgs args) {
			return ResultValue(mEvalMode, tan(args.at(0).floatValue()));
		}, "Computes the tangent of x.") },
		{ "sqrt", Function("sqrt", 1, [this](FnArgs args) {
			return ResultValue(mEvalMode, sqrt(args.at(0).floatValue()));
		}, "Computes the square root of x.") },
		{ "asin", Function("asin", 1, [this](FnArgs args) {
			return ResultValue(mEvalMode, asin(args.at(0).floatValue()));
		}, "Computes the inverse sine of x.") },
		{ "acos", Function("acos", 1, [this](FnArgs args) {
			return ResultValue(mEvalMode, acos(args.at(0).floatValue()));
		}, "Computes the inverse cosine of x.") },
		{ "atan", Function("atan", 1, [this](FnArgs args) {
			return ResultValue(mEvalMode, atan(args.at(0).floatValue()));
		}, "Computes the inverse tangent of x.") },
		{ "ln", Function("ln", 1, [this](FnArgs args) {
			return ResultValue(mEvalMode, log(args.at(0).floatValue()));
		}, "Computes the natrual logaritm of x.") },
		{ "log", Function("log", 1, [this](FnArgs args) {
			return ResultValue(mEvalMode, log10(args.at(0).floatValue()));
		}, "Computes the 10-logaritm of x.") },
		{ "logb", Function("logb", 2, [this](FnArgs args) {
			return ResultValue(mEvalMode, log(args.at(0).floatValue()) / log(args.at(1).floatValue()));
		}, "Computes the y-logaritm of x.") },
		{ "xor", Function("xor", 2, [this](FnArgs args) {
			return ResultValue(mEvalMode, args.at(0).intValue() ^ args.at(1).intValue());
		}, "Bitwise XOR between x and y.") },
		{ "mod", Function("mod", 2, [this](FnArgs args) {
			std::int64_t result = args.at(0).intValue() % args.at(1).intValue();

			if (result < 0) {
				result += args.at(1).intValue();
			}

			return ResultValue(mEvalMode, result);
		}, "Computes x mod y.") },
		{ "ceil", Function("ceil", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, ceil(x.at(0).floatValue()));
		}, "Ceils x.") },
		{ "floor", Function("floor", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, floor(x.at(0).floatValue()));
		}, "Floors x.") },
		{ "round", Function("round", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, round(x.at(0).floatValue()));
		}, "Rounds x.") },
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