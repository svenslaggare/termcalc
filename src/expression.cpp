#include "expression.h"
#include "calcengine.h"
#include <cmath>


//Double expression
DoubleExpression::DoubleExpression(double value): mValue(value) {

}

void DoubleExpression::evaluate(Environment& env, EvalStack& evalStack) const {
	evalStack.push(mValue);
}

//Long expression
LongExpression::LongExpression(long value): mValue(value) {

}

void LongExpression::evaluate(Environment& env, EvalStack& evalStack) const {
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
	ResultValue value;
	if (env.getVariable(mName, value)) {
		evalStack.push(value);
	} else {
		throw std::runtime_error("'" + mName + "' is not a defined variable.");
	}
}

//Function call
FunctionCallExpression::FunctionCallExpression(std::string name, std::vector<std::unique_ptr<Expression>> arguments, const Function& function)
	: mName(name), mArguments(std::move(arguments)), mFunction(function) {

}

void FunctionCallExpression::evaluate(Environment& env, EvalStack& evalStack) const {
	for (auto& arg : mArguments) {
		arg->evaluate(env, evalStack);
	}

	FnArgs args;
	for (std::size_t i = 0; i < mFunction.numArgs(); i++) {
		auto arg = evalStack.top();
		evalStack.pop();
		args.push_back(arg.doubleValue());
	}

	evalStack.push(mFunction.apply(args));
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

			auto value = evalStack.top().doubleValue();
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

		bool floatMode = true;

		if (op1.type() == op2.type()) {
			floatMode = op1.type() == ResultValueType::FLOAT;
		}

		switch (mOp.op()) {
		case '+':
			if (floatMode) {
				evalStack.push(op1.doubleValue() + op2.doubleValue());
			} else {
				evalStack.push(op1.longValue() + op2.longValue());
			}
			break;
		case '-':
			if (floatMode) {
				evalStack.push(op1.doubleValue() - op2.doubleValue());
			} else {
				evalStack.push(op1.longValue() - op2.longValue());
			}
			break;
		case '*':
			if (floatMode) {
				evalStack.push(op1.doubleValue() * op2.doubleValue());
			} else {
				evalStack.push(op1.longValue() * op2.longValue());
			}
			break;
		case '/':
			if (floatMode) {
				evalStack.push(op1.doubleValue() / op2.doubleValue());
			} else {
				evalStack.push(op1.longValue() / op2.longValue());
			}
			break;
		case '%':
			if (floatMode) {
				evalStack.push((double)((long)op1.doubleValue() % (long)op2.doubleValue()));
			} else {
				evalStack.push(op1.longValue() % op2.longValue());
			}
			break;
		case '^':
			if (floatMode) {
				evalStack.push(pow(op1.doubleValue(), op2.doubleValue()));
			} else {
				evalStack.push((long)pow(op1.longValue(), op2.longValue()));
			}
			break;
		}
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

	switch (mOp.op()) {
	case '-':
		if (operand.type() == ResultValueType::FLOAT) {
			evalStack.push(-operand.doubleValue());
		} else {
			evalStack.push(-operand.longValue());
		}
		break;
	}
}