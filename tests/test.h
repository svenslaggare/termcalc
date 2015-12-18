#include <string>
#include <vector>
#include <cxxtest/TestSuite.h>
#include "../src/calcengine.h"
#include "../src/environment.h"
#include "../src/parser.h"

using Tokens = std::vector<Token>;

class TermCalcTestSuite : public CxxTest::TestSuite {
private:
    void printTokens(Tokens tokens) {
        for (auto token : tokens) {
            std::cout << token << std::endl;
        }
    }
public:
    void testTokenizer() {
        TS_ASSERT_EQUALS(
            Tokenizer::tokenize("2.0+5"),
            Tokens({ Token(2.0), Token(TokenType::OPERATOR, '+'), Token(5.0) }));

        TS_ASSERT_EQUALS(
            Tokenizer::tokenize("(2.0+5) * 7.0"),
            Tokens({ Token(TokenType::LEFT_PARENTHESIS), 
                     Token(2.0), Token(TokenType::OPERATOR, '+'), Token(5.0),
                     Token(TokenType::RIGHT_PARENTHESIS),
                     Token(TokenType::OPERATOR, '*'), Token( 7.0) }));

        TS_ASSERT_EQUALS(
            Tokenizer::tokenize("1 << 10"),
            Tokens({ Token(1L), Token(TokenType::TWO_CHAR_OPERATOR, '<', '<'), Token(10L) }));
    }

    void testDifferentBase() {
        TS_ASSERT_EQUALS(
            Tokenizer::tokenize("0x1337"),
            Tokens({ Token(4919L) }));

        TS_ASSERT_EQUALS(
            Tokenizer::tokenize("0b1010100"),
            Tokens({ Token(84L) }));
    }

    void testParseNumbers() {
		TS_ASSERT_EQUALS(
			Tokenizer::tokenize("425354312421461441"),
			Tokens({ Token(425354312421461441) }));
    }

    void testEval() {
        CalcEngine engine;
        TS_ASSERT_EQUALS(engine.eval("3+2"), ResultValue(5.0));
        TS_ASSERT_EQUALS(engine.eval("(2 + 5) * 3"), ResultValue(21.0));
        TS_ASSERT_EQUALS(engine.eval("5 - 3"), ResultValue(2.0));
        TS_ASSERT_EQUALS(engine.eval("2^3"), ResultValue(8.0));
        TS_ASSERT_EQUALS(engine.eval("-3"), ResultValue(-3.0));
    }

    void testEvalVariables() {
        CalcEngine engine;
        TS_ASSERT_DELTA(engine.eval("2 * pi"), ResultValue(6.28), 0.01);

        Environment env;
        env.set("e", 2.718281828);
        TS_ASSERT_DELTA(engine.eval("e^2", env), ResultValue(7.38905609893), 0.01);

        TS_ASSERT_THROWS(engine.eval("2 * x"), std::runtime_error); 
    }

    void testEvalFunctions() {
        CalcEngine engine;
        TS_ASSERT_DELTA(engine.eval("sin(0.5)"), ResultValue(0.479426), 0.01);       
        TS_ASSERT_THROWS(engine.eval("sin(2, 3)"), std::runtime_error);       
        TS_ASSERT_THROWS(engine.eval("f(2, 3)"), std::runtime_error);       
    }

    void testDefineFunctions() {
        CalcEngine engine;
        Environment env;
        engine.eval("f(x)=x^2", env);
        TS_ASSERT_EQUALS(engine.eval("f(4)", env), ResultValue(16.0));   

        engine.setEvalMode(ResultValueType::INTEGER);
        TS_ASSERT_EQUALS(engine.eval("f(4)", env), ResultValue(16L));
    }

	void testDefineFunctions2() {
		CalcEngine engine;
		engine.setEvalMode(ResultValueType::INTEGER);
		Environment env;
		engine.eval("f(x)=x^2", env);
		TS_ASSERT_EQUALS(engine.eval("f(4)", env), ResultValue(16L));
	}

    void testInvalidEval() {
        CalcEngine engine;
        TS_ASSERT_THROWS(engine.eval("3 ++ 2"), std::runtime_error);
        TS_ASSERT_THROWS(engine.eval("3+"), std::runtime_error);
    }

    void testAssignment() {
        CalcEngine engine;
        Environment env;
        TS_ASSERT_EQUALS(engine.eval("x = 2 * 5", env), ResultValue(10.0));
        TS_ASSERT_EQUALS(env.valueOf("x"), ResultValue(10.0));
    }

    void testEvalIntMode() {
        CalcEngine engine;
        engine.setEvalMode(ResultValueType::INTEGER);

        TS_ASSERT_EQUALS(engine.eval("3+2"), ResultValue(5L));
        TS_ASSERT_EQUALS(engine.eval("(2 + 5) * 3"), ResultValue(21L));
        TS_ASSERT_EQUALS(engine.eval("5 - 3"), ResultValue(2L));
        TS_ASSERT_EQUALS(engine.eval("2^3"), ResultValue(8L));
        TS_ASSERT_EQUALS(engine.eval("-3"), ResultValue(-3L));
        TS_ASSERT_EQUALS(engine.eval("2^30"), ResultValue(1L << 30L));

        TS_ASSERT_EQUALS(engine.eval("1 << 30"), ResultValue(1L << 30L));
        TS_ASSERT_EQUALS(engine.eval("24 >> 2"), ResultValue(24L >> 2L));
    }

    void testConvertToInt() {
        CalcEngine engine;
        engine.setEvalMode(ResultValueType::INTEGER);

        TS_ASSERT_EQUALS(engine.eval("pi+2"), ResultValue(5L));
    }
};