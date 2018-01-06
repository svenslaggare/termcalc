#pragma once
#include "dfsvisitor.h"

/**
 * Represents a print visitor
 */
class PrintVisitor : public Visitor {
private:
	CalcEngine& mCalcEngine;
	std::ostream& mStream;

	/**
	 * Indicates if the expression needs parentheses
	 * @param expression The expression
	 * @param op The operator
	 */
	bool needParentheses(Expression* expression, OperatorChar op);
public:
	/**
	 * Creates a new print visitor
	 * @param calcEngine The calc engine
	 * @param stream The stream to print to
	 */
	PrintVisitor(CalcEngine& calcEngine, std::ostream& stream);

	/**
	 * Converts the given expression to a string
	 * @param calcEngine The calc engine
	 * @param expression The expression
	 */
	static std::string toString(CalcEngine& calcEngine, Expression* expression);

	void visit(Expression* parent, NumberExpression* expression) override;
	void visit(Expression* parent, VariableExpression* expression) override;
	void visit(Expression* parent, FunctionCallExpression* expression) override;
	void visit(Expression* parent, BinaryOperatorExpression* expression) override;
	void visit(Expression* parent, UnaryOperatorExpression* expression) override;
};