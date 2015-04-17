#include "expression.h"
#include "calcengine.h"
#include <cmath>

//Number expression
NumberExpression::NumberExpression(double value): mValue(value) {

}

void NumberExpression::evaluate(Environment& env, EvalStack& evalStack) const {
	evalStack.push(mValue);
}

//Variable expression
VariableExpression::VariableExpression(std::string name)
	: mName(name) {

}

std::string VariableExpression::name() const {
	return mName;
}

void VariableExpression::evaluate(Environment& env, EvalStack& evalStack) const {
	double value;
	if (env.getVariable(mName, value)) {
		evalStack.push(value);
	} else {
		throw std::runtime_error("'" + mName + "' is not a defined variable.");
	}
}

//Function call
FunctionCallExpression::FunctionCallExpression(std::string name, std::vector<std::unique_ptr<Expression>> arguments, ApplyUnaryOperator function)
	: mName(name), mArguments(std::move(arguments)), mFunction(function) {

}

void FunctionCallExpression::evaluate(Environment& env, EvalStack& evalStack) const {
	for (auto& arg : mArguments) {
		arg->evaluate(env, evalStack);
	}

	auto arg = evalStack.top();
	evalStack.pop();
	evalStack.push(mFunction(arg));
}

//Binary operator expression
BinaryOperatorExpression::BinaryOperatorExpression(Operator op, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs)
	: mOp(op), mLHS(std::move(lhs)), mRHS(std::move(rhs)) {

}

void BinaryOperatorExpression::evaluate(Environment& env, EvalStack& evalStack) const {
	if (mOp.op() == '=') {
		auto var = dynamic_cast<VariableExpression*>(mLHS.get());

		if (var != nullptr) {
			mRHS->evaluate(env, evalStack);

			auto value = evalStack.top();
			evalStack.pop();

			env.set(var->name(), value);

			evalStack.push(value);
		} else {
			throw std::runtime_error("The left hand side must be a variable.");
		}
	} else {
		mLHS->evaluate(env, evalStack);
		mRHS->evaluate(env, evalStack);

		auto op2 = evalStack.top();
		evalStack.pop();

		auto op1 = evalStack.top();
		evalStack.pop();

		evalStack.push(mOp.apply(op1, op2));
	}
}

//Unary expression
UnaryOperatorExpression::UnaryOperatorExpression(Operator op, std::unique_ptr<Expression> operand)
	: mOp(op), mOperand(std::move(operand)) {

}

void UnaryOperatorExpression::evaluate(Environment& env, EvalStack& evalStack) const {
	mOperand->evaluate(env, evalStack);

	auto operand = evalStack.top();
	evalStack.pop();

	evalStack.push(mOp.apply(operand));
}