#include "expression.h"
#include "calcengine.h"
#include "environment.h"
#include "function.h"
#include <cmath>
#include <unordered_set>
#include <sstream>

//Number expression
NumberExpression::NumberExpression(NumericConstant value)
	: mValue(value) {

}

std::string NumberExpression::toString() {
	return mValue.toString();
}

void NumberExpression::evaluate(CalcEngine& calcEngine, Environment& env, EvalStack& evalStack) {
//	evalStack.push(mValue);
	evalStack.push(calcEngine.currentNumberType().toResultValue(mValue));
}

//Variable expression
VariableExpression::VariableExpression(std::string name)
	: mName(name) {

}

std::string VariableExpression::name() const {
	return mName;
}

std::string VariableExpression::toString() {
	return mName;
}

void VariableExpression::evaluate(CalcEngine& calcEngine, Environment& env, EvalStack& evalStack) {
	ResultValue value;
	if (env.getVariable(mName, value, true)) {
		evalStack.push(value);
	} else {
		throw std::runtime_error("'" + mName + "' is not a defined variable. Type ':help' for a list of commands.");
	}
}

//Function call
FunctionCallExpression::FunctionCallExpression(std::string name, std::vector<std::unique_ptr<Expression>> arguments)
	: mName(name), mArguments(std::move(arguments)) {

}

std::string FunctionCallExpression::name() const {
	return mName;
}

std::size_t FunctionCallExpression::numArguments() const {
	return mArguments.size();
}

Expression* FunctionCallExpression::getArgument(std::size_t index) const {
	if (index >= mArguments.size()) {
		throw std::runtime_error("Invalid argument index.");
	}

	return mArguments[index].get();
}

std::string FunctionCallExpression::toString() {
	std::ostringstream stream;

	stream << mName << "(";

	bool isFirst = true;

	for (auto& arg : mArguments) {
		if (!isFirst) {
			stream << ", ";
		}

		stream << arg->toString();

		isFirst = false;
	}

	stream << ")";

	return stream.str();
}

void FunctionCallExpression::evaluate(CalcEngine& calcEngine, Environment& env, EvalStack& evalStack) {
	//Find the function
	auto& func = env.getFunction(mName, mArguments.size());

	if (mArguments.size() != func.numArgs()) {
		throw std::runtime_error(
				"Expected " + std::to_string(func.numArgs())
				+ " arguments but got " + std::to_string(mArguments.size()));
	}

	for (auto& arg : mArguments) {
		arg->evaluate(calcEngine, env, evalStack);
	}

	FnArgs args;
	for (std::size_t i = 0; i < func.numArgs(); i++) {
		auto arg = evalStack.top();
		evalStack.pop();
		args.insert(args.begin(), arg);
	}

	evalStack.push(func.apply(calcEngine, env, args));
}

//Binary operator expression
BinaryOperatorExpression::BinaryOperatorExpression(OperatorChar op, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs)
	: mOp(op), mLHS(std::move(lhs)), mRHS(std::move(rhs)) {

}

std::string BinaryOperatorExpression::toString() {
	return mLHS->toString() + mOp.toString() + mRHS->toString();
}

void BinaryOperatorExpression::evaluate(CalcEngine& calcEngine, Environment& env, EvalStack& evalStack) {
	if (mOp == '=') {
		if (auto var = dynamic_cast<VariableExpression*>(mLHS.get())) {
			mRHS->evaluate(calcEngine, env, evalStack);

			auto value = evalStack.top().floatValue();
			evalStack.pop();

			env.set(var->name(), value);

			evalStack.push(value);
		} else if (auto func = dynamic_cast<FunctionCallExpression*>(mLHS.get())) {
			//Check that the parameters are variables
			std::unordered_set<std::string> usedParameterNames;
			std::vector<std::string> parameters;

			for (std::size_t i = 0; i < func->numArguments(); i++) {
				if (auto param = dynamic_cast<VariableExpression*>(func->getArgument(i))) {
					if (usedParameterNames.count(param->name()) == 0) {
						usedParameterNames.insert(param->name());
						parameters.push_back(param->name());
					} else {
						throw std::runtime_error("The parameter '" + param->name() + "' is already used.");
					}
				} else {
					throw std::runtime_error("Parameter number " + std::to_string(i) + " is not a variable.");
				}
			}

			env.define(Function(func->name(), parameters.size(), std::make_shared<FunctionBody>(parameters, std::move(mRHS))));
			evalStack.push(ResultValue());
		} else {
			throw std::runtime_error("The left hand side must be a variable or a function definition.");
		}
	} else {
		//It's possible that the expression was defined using a type that don't support the current operator.
		if (calcEngine.binaryOperators().count(mOp) == 0) {
			throw std::runtime_error("The current mode does not support the operator '" + mOp.toString() + "'.");
		}

		auto& op = calcEngine.binaryOperators().at(mOp);

		mLHS->evaluate(calcEngine, env, evalStack);
		mRHS->evaluate(calcEngine, env, evalStack);

		auto op2 = evalStack.top();
		evalStack.pop();

		auto op1 = evalStack.top();
		evalStack.pop();

		evalStack.push(op.apply(op1, op2));
	}
}

//Unary expression
UnaryOperatorExpression::UnaryOperatorExpression(OperatorChar op, std::unique_ptr<Expression> operand)
	: mOp(op), mOperand(std::move(operand)) {

}

std::string UnaryOperatorExpression::toString() {
	return mOp.toString() + mOperand->toString();
}

void UnaryOperatorExpression::evaluate(CalcEngine& calcEngine, Environment& env, EvalStack& evalStack) {
	//It's possible that the expression was defined using a type that don't support the current operator.
	if (calcEngine.unaryOperators().count(mOp) == 0) {
		throw std::runtime_error("The current mode does not support the operator '" + mOp.toString() + "'.");
	}

	auto& op = calcEngine.unaryOperators().at(mOp);

	mOperand->evaluate(calcEngine, env, evalStack);

	auto operand = evalStack.top();
	evalStack.pop();

	evalStack.push(op.apply(operand));
}