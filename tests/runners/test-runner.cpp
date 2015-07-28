/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#define _CXXTEST_HAVE_EH
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_TermCalcTestSuite_init = false;
#include "/home/antjans/Code/termcalc/tests/test.h"

static TermCalcTestSuite suite_TermCalcTestSuite;

static CxxTest::List Tests_TermCalcTestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TermCalcTestSuite( "tests/test.h", 9, "TermCalcTestSuite", suite_TermCalcTestSuite, Tests_TermCalcTestSuite );

static class TestDescription_suite_TermCalcTestSuite_testTokenizer : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TermCalcTestSuite_testTokenizer() : CxxTest::RealTestDescription( Tests_TermCalcTestSuite, suiteDescription_TermCalcTestSuite, 17, "testTokenizer" ) {}
 void runTest() { suite_TermCalcTestSuite.testTokenizer(); }
} testDescription_suite_TermCalcTestSuite_testTokenizer;

static class TestDescription_suite_TermCalcTestSuite_testDifferentBase : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TermCalcTestSuite_testDifferentBase() : CxxTest::RealTestDescription( Tests_TermCalcTestSuite, suiteDescription_TermCalcTestSuite, 30, "testDifferentBase" ) {}
 void runTest() { suite_TermCalcTestSuite.testDifferentBase(); }
} testDescription_suite_TermCalcTestSuite_testDifferentBase;

static class TestDescription_suite_TermCalcTestSuite_testEval : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TermCalcTestSuite_testEval() : CxxTest::RealTestDescription( Tests_TermCalcTestSuite, suiteDescription_TermCalcTestSuite, 40, "testEval" ) {}
 void runTest() { suite_TermCalcTestSuite.testEval(); }
} testDescription_suite_TermCalcTestSuite_testEval;

static class TestDescription_suite_TermCalcTestSuite_testEvalVariables : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TermCalcTestSuite_testEvalVariables() : CxxTest::RealTestDescription( Tests_TermCalcTestSuite, suiteDescription_TermCalcTestSuite, 49, "testEvalVariables" ) {}
 void runTest() { suite_TermCalcTestSuite.testEvalVariables(); }
} testDescription_suite_TermCalcTestSuite_testEvalVariables;

static class TestDescription_suite_TermCalcTestSuite_testEvalFunctions : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TermCalcTestSuite_testEvalFunctions() : CxxTest::RealTestDescription( Tests_TermCalcTestSuite, suiteDescription_TermCalcTestSuite, 58, "testEvalFunctions" ) {}
 void runTest() { suite_TermCalcTestSuite.testEvalFunctions(); }
} testDescription_suite_TermCalcTestSuite_testEvalFunctions;

static class TestDescription_suite_TermCalcTestSuite_testInvalidEval : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TermCalcTestSuite_testInvalidEval() : CxxTest::RealTestDescription( Tests_TermCalcTestSuite, suiteDescription_TermCalcTestSuite, 63, "testInvalidEval" ) {}
 void runTest() { suite_TermCalcTestSuite.testInvalidEval(); }
} testDescription_suite_TermCalcTestSuite_testInvalidEval;

static class TestDescription_suite_TermCalcTestSuite_testAssignment : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TermCalcTestSuite_testAssignment() : CxxTest::RealTestDescription( Tests_TermCalcTestSuite, suiteDescription_TermCalcTestSuite, 69, "testAssignment" ) {}
 void runTest() { suite_TermCalcTestSuite.testAssignment(); }
} testDescription_suite_TermCalcTestSuite_testAssignment;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
