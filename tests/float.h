#include <string>
#include <vector>
#include <cxxtest/TestSuite.h>
#include "../src/calcengine.h"
#include "../src/environment.h"
#include "../src/parser.h"
#include "../src/numbertype.h"

using Tokens = std::vector<Token>;

class FloatTestSuite : public CxxTest::TestSuite {
public:
	void testEval() {
		CalcEngine engine;
		TS_ASSERT_EQUALS(engine.eval("3+2"), ResultValue(5.0));
		TS_ASSERT_EQUALS(engine.eval("(2 + 5) * 3"), ResultValue(21.0));
		TS_ASSERT_EQUALS(engine.eval("5 - 3"), ResultValue(2.0));
		TS_ASSERT_EQUALS(engine.eval("2^3"), ResultValue(8.0));
		TS_ASSERT_EQUALS(engine.eval("-3"), ResultValue(-3.0));
	}

	void testEvalFunctions() {
		CalcEngine engine;
		TS_ASSERT_DELTA(engine.eval("sin(0.5)").floatValue(), ResultValue(0.479426).floatValue(), 0.01);
		TS_ASSERT_THROWS(engine.eval("sin(2, 3)"), std::runtime_error);
		TS_ASSERT_THROWS(engine.eval("f(2, 3)"), std::runtime_error);
	}
};