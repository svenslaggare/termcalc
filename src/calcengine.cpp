#include "calcengine.h"
#include "parser.h"
#include "expression.h"
#include "operator.h"
#include <stdexcept>
#include <cmath>
#include <queue>
#include <stack>

//Environment
Environment::Environment() {
	mValues = {
		{ "pi", ResultValue(3.141592653589793238463) }
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

ResultValue Environment::valueOf(std::string variable) const {
	return mValues.at(variable);
}

void Environment::setEvalMode(ResultValueType evalMode) {
	mEvalMode = evalMode;
}

//Calc engine
CalcEngine::CalcEngine()
	: mEvalMode(ResultValueType::FLOAT) {
	mBinaryOperators = {
		{ '^', Operator('^', 6, OperatorAssociativity::RIGHT) },
		{ '*', Operator('*', 5, OperatorAssociativity::LEFT) },
		{ '/', Operator('/', 5, OperatorAssociativity::LEFT) },
		{ '%', Operator('%', 5, OperatorAssociativity::LEFT) },
		{ '+', Operator('+', 4, OperatorAssociativity::LEFT) },
		{ '-', Operator('-', 4, OperatorAssociativity::LEFT) },
		{ '|', Operator('|', 2, OperatorAssociativity::LEFT) },
		{ '&', Operator('&', 2, OperatorAssociativity::LEFT) },
		{ '=', Operator('=', 1, OperatorAssociativity::RIGHT) }
	};

	mUnaryOperators = {
		{ '-', Operator('-', 7, OperatorAssociativity::LEFT, true) },
		{ '~', Operator('~', 7, OperatorAssociativity::LEFT, true) },
	};

	mFunctions = {
		{ "sin", Function("sin", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, sin(x.at(0).doubleValue()));
		}, "Computes the sine of x.") },
		{ "cos", Function("cos", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, cos(x.at(0).doubleValue()));
		}, "Computes the cosine of x.") },
		{ "tan", Function("tan", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, tan(x.at(0).doubleValue()));
		}, "Computes the tangent of x.") },
		{ "sqrt", Function("sqrt", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, sqrt(x.at(0).doubleValue()));
		}, "Computes the square root of x.") },
		{ "asin", Function("asin", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, asin(x.at(0).doubleValue()));
		}, "Computes the inverse sine of x.") },
		{ "acos", Function("acos", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, acos(x.at(0).doubleValue()));
		}, "Computes the inverse cosine of x.") },
		{ "atan", Function("atan", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, atan(x.at(0).doubleValue()));
		}, "Computes the inverse tangent of x.") },
		{ "ln", Function("ln", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, log(x.at(0).doubleValue()));
		}, "Computes the natrual logaritm of x.") },
		{ "log", Function("log", 1, [this](FnArgs x) {
			return ResultValue(mEvalMode, log10(x.at(0).doubleValue()));
		}, "Computes the 10-logaritm of x.") },
		{ "logb", Function("logb", 2, [this](FnArgs x) {
			return ResultValue(mEvalMode, log(x.at(0).doubleValue()) / log(x.at(1).doubleValue()));
		}, "Computes the y-logaritm of x.") },
		{ "xor", Function("xor", 2, [this](FnArgs x) {
			return ResultValue(mEvalMode, x.at(0).longValue() ^ x.at(1).longValue());
		}, "Bitwise XOR between x and y.") },
		{ "sr", Function("sr", 2, [this](FnArgs x) {
			return ResultValue(mEvalMode, x.at(0).longValue() >> x.at(1).longValue());
		}, "Shifts x, y bits to the right.") },
		{ "sl", Function("sl", 2, [this](FnArgs x) {
			return ResultValue(mEvalMode, x.at(0).longValue() << x.at(1).longValue());
		}, "Shifts x, y bits to the left.") },
		{ "mod", Function("mod", 2, [this](FnArgs x) {
			long result = x.at(0).longValue() % x.at(1).longValue();

			if (result < 0) {
				result += x.at(1).longValue();
			}

			return ResultValue(mEvalMode, result);
		}, "Computes x mod y.") },
	};
}

const std::unordered_map<char, Operator>& CalcEngine::binaryOperators() const {
	return mBinaryOperators;
}

const std::unordered_map<char, Operator>& CalcEngine::unaryOperators() const {
	return mUnaryOperators;
}

const std::unordered_map<std::string, Function>& CalcEngine::functions() const {
	return mFunctions;
}

ResultValueType CalcEngine::evalMode() const {
	return mEvalMode;
}

void CalcEngine::setEvalMode(ResultValueType evalMode) {
	mEvalMode = evalMode;
}

ResultValue CalcEngine::eval(std::string expressionString) {
	Environment env;
	env.setEvalMode(mEvalMode);
	return eval(expressionString, env);
}

ResultValue CalcEngine::eval(std::string expressionString, Environment& env) {
	env.setEvalMode(mEvalMode);

	//Tokenize
	auto tokens = Tokenizer::tokenize(expressionString);
	tokens.push_back(Token(TokenType::END_OF_EXPRESSION));

	//Parse
	Parser parser(tokens, *this);
	auto expression = parser.parse();
	EvalStack evalStack;

	//Evaluate
	expression->evaluate(env, evalStack);

	if (evalStack.empty()) {
		throw std::runtime_error("Expected result.");
	}

	return evalStack.top();
}
