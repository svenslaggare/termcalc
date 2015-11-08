#include "calcengine.h"
#include "parser.h"
#include "expression.h"
#include "operator.h"
#include "environment.h"
#include <stdexcept>
#include <queue>
#include <stack>
#include <cmath>

//Calc engine
CalcEngine::CalcEngine()
	: mEvalMode(ResultValueType::FLOAT) {
	mBinaryOperators = { {
		{ '^', Operator('^', 6, OperatorAssociativity::RIGHT, [](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			if (evalMode == ResultValueType::FLOAT) {
				return ResultValue(pow(lhs.floatValue(), rhs.floatValue()));
			} else {
				return ResultValue((std::int64_t)pow(lhs.intValue(), rhs.intValue()));
			}
		}) },
		{ '*', Operator('*', 5, OperatorAssociativity::LEFT, [](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			if (evalMode == ResultValueType::FLOAT) {
				return ResultValue(lhs.floatValue() * rhs.floatValue());
			} else {
				return ResultValue(lhs.intValue() * rhs.intValue());
			}
		}) },
		{ '/', Operator('/', 5, OperatorAssociativity::LEFT, [](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			if (evalMode == ResultValueType::FLOAT) {
				return ResultValue(lhs.floatValue() / rhs.floatValue());
			} else {
				return ResultValue(lhs.intValue() / rhs.intValue());
			}
		}) },
		{ '%', Operator('%', 5, OperatorAssociativity::LEFT, [](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			return ResultValue();
		}) },
		{ '+', Operator('+', 4, OperatorAssociativity::LEFT, [](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			if (evalMode == ResultValueType::FLOAT) {
				return ResultValue(lhs.floatValue() + rhs.floatValue());
			} else {
				return ResultValue(lhs.intValue() + rhs.intValue());
			}
		}) },
		{ '-', Operator('-', 4, OperatorAssociativity::LEFT, [](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			if (evalMode == ResultValueType::FLOAT) {
				return ResultValue(lhs.floatValue() - rhs.floatValue());
			} else {
				return ResultValue(lhs.intValue() - rhs.intValue());
			}
		}) },
		{ OperatorChar('<', '<'), Operator(OperatorChar('<', '<'), 3, OperatorAssociativity::LEFT, [](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			if (evalMode == ResultValueType::FLOAT) {
				return ResultValue((double)((std::int64_t)lhs.floatValue() << (std::int64_t)rhs.floatValue()));
			} else {
				return ResultValue(lhs.intValue() << rhs.intValue());
			}
		}) },
		{ OperatorChar('>', '>'), Operator(OperatorChar('>', '>'), 3, OperatorAssociativity::LEFT, [](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			if (evalMode == ResultValueType::FLOAT) {
				return ResultValue((double)((std::int64_t)lhs.floatValue() >> (std::int64_t)rhs.floatValue()));
			} else {
				return ResultValue(lhs.intValue() >> rhs.intValue());
			}
		}) },
		{ '|', Operator('|', 2, OperatorAssociativity::LEFT, [](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			if (evalMode == ResultValueType::FLOAT) {
				return ResultValue((double)((std::int64_t)lhs.floatValue() | (std::int64_t)rhs.floatValue()));
			} else {
				return ResultValue(lhs.intValue() | rhs.intValue());
			}
		}) },
		{ '&', Operator('&', 2, OperatorAssociativity::LEFT, [](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			if (evalMode == ResultValueType::FLOAT) {
				return ResultValue((double)((std::int64_t)lhs.floatValue() & (std::int64_t)rhs.floatValue()));
			} else {
				return ResultValue(lhs.intValue() & rhs.intValue());
			}
		}) },
		{ '=', Operator('=', 1, OperatorAssociativity::RIGHT, [](ResultValueType evalMode, ResultValue lhs, ResultValue rhs) {
			return ResultValue();
		}) }
	}, 15, OperatorChar::HASH, OperatorChar::EQUAL };

	mUnaryOperators = {
		{ '-', Operator('-', 7, OperatorAssociativity::LEFT, [](ResultValueType evalMode, ResultValue op) {
			if (evalMode == ResultValueType::FLOAT) {
				return ResultValue(-op.floatValue());
			} else {
				return ResultValue(-op.intValue());
			}
		}) },
		{ '~', Operator('~', 7, OperatorAssociativity::LEFT, [](ResultValueType evalMode, ResultValue op) {
			if (evalMode == ResultValueType::FLOAT) {
				return ResultValue((double)(~(std::int64_t)op.floatValue()));
			} else {
				return ResultValue(~op.intValue());
			}
		}) },
	};
}

const CalcEngine::BinaryOperators& CalcEngine::binaryOperators() const {
	return mBinaryOperators;
}

const std::unordered_map<char, Operator>& CalcEngine::unaryOperators() const {
	return mUnaryOperators;
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
