#include <string>
#include <vector>
#include <cxxtest/TestSuite.h>
#include "../src/commandengine.h"
#include "../src/calcengine.h"
#include "../src/environment.h"
#include "../src/parser.h"
#include "../src/numbertype.h"

using Tokens = std::vector<Token>;

class IntTestSuite : public CxxTest::TestSuite {
public:
	void testDifferentBase() {
		IntegerType integerType;

		TS_ASSERT_EQUALS(
			Tokenizer::tokenize("0x1337", integerType),
			Tokens({ Token(4919LL) }));

		TS_ASSERT_EQUALS(
			Tokenizer::tokenize("0b1010100", integerType),
			Tokens({ Token(84LL) }));
	}

	void testLargeValues() {
		IntegerType integerType;

		TS_ASSERT_EQUALS(
			Tokenizer::tokenize("425354312421461441", integerType),
			Tokens({ Token(425354312421461441LL) }));
	}

	void testEval() {
		CalcEngine engine;
		engine.setEvalMode(ResultValueType::INTEGER);

		TS_ASSERT_EQUALS(engine.eval("3+2"), ResultValue(5LL));
		TS_ASSERT_EQUALS(engine.eval("(2 + 5) * 3"), ResultValue(21LL));
		TS_ASSERT_EQUALS(engine.eval("5 - 3"), ResultValue(2LL));
		TS_ASSERT_EQUALS(engine.eval("2^3"), ResultValue(8LL));
		TS_ASSERT_EQUALS(engine.eval("-3"), ResultValue(-3LL));
		TS_ASSERT_EQUALS(engine.eval("2^30"), ResultValue(1LL << 30LL));

		TS_ASSERT_EQUALS(engine.eval("1 << 30"), ResultValue(1LL << 30LL));
		TS_ASSERT_EQUALS(engine.eval("24 >> 2"), ResultValue(24LL >> 2LL));
	}

	void testDefineFunctions() {
		CalcEngine engine;
		engine.setEvalMode(ResultValueType::INTEGER);
		Environment env;
		engine.eval("f(x)=x^2", env);
		TS_ASSERT_EQUALS(engine.eval("f(4)", env), ResultValue(16LL));
	}
};