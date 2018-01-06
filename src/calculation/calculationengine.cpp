#include "calculationengine.h"
#include "../parser/parser.h"
#include "../expressions/expression.h"
#include "../core/operator.h"
#include "environment.h"
#include "calculationvisitor.h"

#include <stdexcept>
#include <queue>
#include <stack>
#include <cmath>

//Calc engine
CalculationEngine::CalculationEngine(std::ostream& os)
	: mEvalMode(ResultValueType::FLOAT) {
	mNumberTypes.emplace(ResultValueType::FLOAT, std::unique_ptr<FloatType>(new FloatType));
	mNumberTypes.emplace(ResultValueType::INTEGER, std::unique_ptr<IntegerType>(new IntegerType(os)));
	mNumberTypes.emplace(ResultValueType::COMPLEX, std::unique_ptr<ComplexType>(new ComplexType(os)));
}

NumberType& CalculationEngine::currentNumberType() const {
	return *mNumberTypes.at(mEvalMode).get();
}

const BinaryOperators& CalculationEngine::binaryOperators() const {
	return currentNumberType().binaryOperators();
}

bool CalculationEngine::getBinaryOperator(OperatorChar opChar, Operator& op) const {
	auto operatorIterator = binaryOperators().find(opChar);
	if (operatorIterator != binaryOperators().end()) {
		op = operatorIterator->second;
		return true;
	}

	return false;
}

const UnaryOperators& CalculationEngine::unaryOperators() const {
	return currentNumberType().unaryOperators();
}

Environment CalculationEngine::defaultEnvironment() const {
	std::unordered_map<ResultValueType, EnvironmentScope> scopes;
	for (auto& numberType : mNumberTypes) {
		scopes.insert({ numberType.first, numberType.second->environment() });
	}

	return Environment(scopes);
}

ResultValueType CalculationEngine::evalMode() const {
	return mEvalMode;
}

void CalculationEngine::setEvalMode(ResultValueType evalMode) {
	mEvalMode = evalMode;
}

ResultValue CalculationEngine::evaluate(const std::string& expressionString) {
	auto environment = defaultEnvironment();
	return evaluate(expressionString, environment);
}

ResultValue CalculationEngine::evaluate(const std::string& expressionString, Environment& environment) {
	environment.setEvalMode(mEvalMode);

	//Tokenize
	auto tokens = Tokenizer::tokenize(expressionString, currentNumberType());
	tokens.push_back(Token(TokenType::END_OF_EXPRESSION));

	//Parse
	Parser parser(tokens, *this);
	auto expression = parser.parse();

	//Evaluate
	CalculationVisitor calculationVisitor(*this, environment);
	expression->accept(calculationVisitor, nullptr);

	auto& evaluationStack = calculationVisitor.evaluationStack();
	if (evaluationStack.empty()) {
		throw std::runtime_error("Expected result.");
	}

	return evaluationStack.top();
}
