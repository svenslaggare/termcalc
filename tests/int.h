#include <string>
#include <vector>
#include <cxxtest/TestSuite.h>
#include "../src/calculator.h"
#include "../src/calculation/calculationengine.h"
#include "../src/calculation/environment.h"
#include "../src/parser/parser.h"
#include "../src/calculation/numbertypes/numbertype.h"

using Tokens = std::vector<Token>;

class IntTestSuite : public CxxTest::TestSuite {
public:
	NumericConstant constantInBase(NumericChars chars, unsigned char base = 10) {
		NumericConstant numConstant(chars);
		numConstant.base(base);
		return numConstant;
	}

	void testDifferentBase() {
		IntegerType integerType;
		using namespace NumericConstantChars;

		TS_ASSERT_EQUALS(
			Tokenizer::tokenize("0x1337af", integerType),
			Tokens({
				Token(constantInBase(
				{ One, Three, Three, Seven, A, F },
			    16))
			}));

		TS_ASSERT_EQUALS(
			Tokenizer::tokenize("0b1010100", integerType),
			Tokens({
			   Token(constantInBase(
			   { One, Zero, One, Zero, One, Zero, Zero },
			   2))
		    }));

		TS_ASSERT_EQUALS(0x1337af, integerType.toResultValue(integerType.toNumericConstant("0x1337af")).intValue());
		TS_ASSERT_EQUALS(0b1010100, integerType.toResultValue(integerType.toNumericConstant("0b1010100")).intValue());
	}

	void testLargeValues() {
		IntegerType integerType;

		TS_ASSERT_EQUALS(
			Tokenizer::tokenize("425354312421461441", integerType),
			Tokens({ Token((std::int64_t)425354312421461441LL) }));
	}

	void testEval() {
		CalculationEngine engine;
		engine.setEvalMode(ResultValueType::INTEGER);

		TS_ASSERT_EQUALS(engine.evaluate("3+2"), ResultValue((std::int64_t)5LL));
		TS_ASSERT_EQUALS(engine.evaluate("(2 + 5) * 3"), ResultValue((std::int64_t)21LL));
		TS_ASSERT_EQUALS(engine.evaluate("5 - 3"), ResultValue((std::int64_t)2LL));
		TS_ASSERT_EQUALS(engine.evaluate("2^3"), ResultValue((std::int64_t)8LL));
		TS_ASSERT_EQUALS(engine.evaluate("-3"), ResultValue((std::int64_t)-3LL));
		TS_ASSERT_EQUALS(engine.evaluate("2^30"), ResultValue((std::int64_t)(1LL << 30LL)));

		TS_ASSERT_EQUALS(engine.evaluate("1 << 30"), ResultValue((std::int64_t)(1LL << 30LL)));
		TS_ASSERT_EQUALS(engine.evaluate("24 >> 2"), ResultValue((std::int64_t)(24LL >> 2LL)));
	}

	void testDefineFunctions() {
		CalculationEngine engine;
		engine.setEvalMode(ResultValueType::INTEGER);
		Environment env;
		engine.evaluate("f(x)=x^2", env);
		TS_ASSERT_EQUALS(engine.evaluate("f(4)", env), ResultValue((std::int64_t)16LL));
	}
};