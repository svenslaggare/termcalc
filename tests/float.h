#include <string>
#include <vector>
#include <memory>
#include <cxxtest/TestSuite.h>
#include "../src/calculation/calculationengine.h"
#include "../src/calculation/environment.h"
#include "../src/expressions/expression.h"
#include "../src/parser/parser.h"
#include "../src/calculation/numbertypes/numbertype.h"

using Tokens = std::vector<Token>;

class FloatTestSuite : public CxxTest::TestSuite {
public:
	void testEval() {
		CalculationEngine engine;
		TS_ASSERT_EQUALS(engine.evaluate("3+2"), ResultValue(5.0));
		TS_ASSERT_EQUALS(engine.evaluate("(2 + 5) * 3"), ResultValue(21.0));
		TS_ASSERT_EQUALS(engine.evaluate("5 - 3"), ResultValue(2.0));
		TS_ASSERT_EQUALS(engine.evaluate("2^3"), ResultValue(8.0));
		TS_ASSERT_EQUALS(engine.evaluate("-3"), ResultValue(-3.0));
	}

	void testEvalFunctions() {
		CalculationEngine engine;
		TS_ASSERT_DELTA(engine.evaluate("sin(0.5)").floatValue(), ResultValue(0.479426).floatValue(), 0.01);
		TS_ASSERT_THROWS(engine.evaluate("sin(2, 3)"), std::runtime_error);
		TS_ASSERT_THROWS(engine.evaluate("f(2, 3)"), std::runtime_error);
	}

	void testDefineFunctions() {
		CalculationEngine engine;
		Environment environment;
		environment.define(Function(
			"f1",
			1,
			std::make_shared<UserFunction>(
			    std::vector<std::string> {"x"},
			    std::make_unique<VariableExpression>("x")
            )
		));

        environment.define(Function(
			"f2",
			1,
			std::make_shared<UserFunction>(
			    std::vector<std::string> {"x"},
			    std::make_unique<BinaryOperatorExpression>(
			        OperatorChar {'*'},
			        std::make_unique<NumberExpression>(NumericConstant({NumericConstantChar::Two})),
			        std::make_unique<VariableExpression>("x")
                )
            )
		));
			
		 TS_ASSERT_DELTA(engine.evaluate("f2(275) / f1(167)", environment).floatValue(), ResultValue(3.2934).floatValue(), 0.01);
	}
};