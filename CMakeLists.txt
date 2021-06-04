cmake_minimum_required(VERSION 3.3)
project(termcalc)

#if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O2 -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -std=c++11")

set(SOURCE_FILES
        src/calculation/calculationengine.cpp
        src/calculation/calculationengine.h
        src/calculator.cpp
        src/calculator.h
        src/calculation/environment.cpp
        src/calculation/environment.h
        src/expressions/expression.cpp
        src/expressions/expression.h
        src/calculation/function.cpp
        src/calculation/function.h
        src/core/operator.cpp
        src/core/operator.h
        src/parser/parser.cpp
        src/parser/parser.h
        src/core/resultvalue.cpp
        src/core/resultvalue.h
        src/parser/token.cpp
        src/parser/token.h
        src/linenoise.cpp
        src/linenoise.h
        src/calculation/numbertypes/numbertype.cpp
        src/calculation/numbertypes/numbertype.h
        src/calculation/numberhelpers.cpp
        src/calculation/numberhelpers.h
        src/core/numericconstant.cpp
        src/core/numericconstant.h
        src/termcalc.h
        src/visitors/visitor.cpp
        src/visitors/visitor.h
        src/visitors/dfsvisitor.cpp
        src/visitors/dfsvisitor.h
        src/calculation/calculationvisitor.cpp
        src/calculation/calculationvisitor.h
        src/visitors/printvisitor.cpp
        src/visitors/printvisitor.h
        src/visitors/predicatevisitor.cpp
        src/visitors/predicatevisitor.h
        src/visitors/printvisitor.cpp
        src/visitors/printvisitor.h
        src/calculation/numbertypes/inttype.cpp
        src/calculation/numbertypes/inttype.h
        src/calculation/numbertypes/floattype.cpp
        src/calculation/numbertypes/floattype.h
        src/calculation/numbertypes/complextype.cpp
        src/calculation/numbertypes/complextype.h
        src/calculation/numbertypes/numbertypes.h)

add_executable(termcalc ${SOURCE_FILES} src/termcalc.cpp)

# Tests
set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

find_package(CxxTest)
if(CXXTEST_FOUND)
    add_custom_target(
            check ${CMAKE_COMMAND} -E env CTEST_OUTPUT_ON_FAILURE=1
            ${CMAKE_CTEST_COMMAND} -C $<CONFIG> --verbose
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

    add_library(TestLib STATIC ${SOURCE_FILES})

    include_directories(${CXXTEST_INCLUDE_DIR})
    enable_testing()

    add_executable(tests-dummy ${TESTS_DIR}/dummy.cpp)

    CXXTEST_ADD_TEST(tests-general test-general-runner.cpp ${TESTS_DIR}/general.h)
    target_link_libraries(tests-general TestLib)

    CXXTEST_ADD_TEST(tests-int test-int-runner.cpp ${TESTS_DIR}/int.h)
    target_link_libraries(tests-int TestLib)

    CXXTEST_ADD_TEST(tests-float test-float-runner.cpp ${TESTS_DIR}/float.h)
    target_link_libraries(tests-float TestLib)

    CXXTEST_ADD_TEST(tests-complex test-complex-runner.cpp ${TESTS_DIR}/complex.h)
    target_link_libraries(tests-complex TestLib)

    CXXTEST_ADD_TEST(tests-calculator test-calculator-runner.cpp ${TESTS_DIR}/calculator.h)
    target_link_libraries(tests-calculator TestLib)

    CXXTEST_ADD_TEST(tests-numericconstant test-numericconstant-runner.cpp ${TESTS_DIR}/numericconstant.h)
    target_link_libraries(tests-numericconstant TestLib)
endif()
