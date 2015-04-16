#include <string>
#include <vector>
#include <cxxtest/TestSuite.h>
#include "../src/calcengine.h"

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
        CalcEngine engine;
        TS_ASSERT_EQUALS(
            engine.tokenize("2.0+5"),
            Tokens({ Token(2.0), Token(TokenType::OPERATOR, '+'), Token(5.0) }));

        TS_ASSERT_EQUALS(
            engine.tokenize("(2.0+5) * 7.0"),
            Tokens({ Token(TokenType::LEFT_PARENTHESIS), 
                     Token(2.0), Token(TokenType::OPERATOR, '+'), Token(5.0),
                     Token(TokenType::RIGHT_PARENTHESIS),
                     Token(TokenType::OPERATOR, '*'), Token( 7.0) }));
    }

    void testDifferentBase() {
        CalcEngine engine;
        TS_ASSERT_EQUALS(
            engine.tokenize("0x1337"),
            Tokens({ Token(4919) }));

        TS_ASSERT_EQUALS(
            engine.tokenize("0b1010100"),
            Tokens({ Token(84) }));
    }

    void testEval() {
        CalcEngine engine;
        TS_ASSERT_EQUALS(engine.eval("3+2"), 5);
        TS_ASSERT_EQUALS(engine.eval("(2 + 5) * 3"), 21);
        TS_ASSERT_EQUALS(engine.eval("5 - 3"), 2);
        TS_ASSERT_EQUALS(engine.eval("2^3"), 8);
        TS_ASSERT_EQUALS(engine.eval("-3"), -3);
    }

    void testEvalVariables() {
        CalcEngine engine;
        TS_ASSERT_DELTA(engine.eval("2 * pi"), 6.28, 0.1);

        Environment env;
        env.set("e", 2.718281828);
        TS_ASSERT_DELTA(engine.eval("e^2", env), 7.38905609893, 0.1);
    }

    void testEvalFunctions() {
        CalcEngine engine;
        TS_ASSERT_DELTA(engine.eval("sin(0.5)"), 0.479426, 0.1);       
    }

    void testInvalidEval() {
        CalcEngine engine;
        TS_ASSERT_THROWS(engine.eval("3 ++ 2"), std::runtime_error);
        TS_ASSERT_THROWS(engine.eval("e^2"), std::runtime_error);
    }

    void testAssignment() {
        CalcEngine engine;
        Environment env;
        TS_ASSERT_EQUALS(engine.eval("x = 2 * 5", env), 10);
        TS_ASSERT_EQUALS(env.valueOf("x"), 10);
    }
};