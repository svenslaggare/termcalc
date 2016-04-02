cmake_minimum_required(VERSION 3.3)
project(termcalc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/calcengine.cpp
    src/calcengine.h
    src/commandengine.cpp
    src/commandengine.h
    src/environment.cpp
    src/environment.h
    src/expression.cpp
    src/expression.h
    src/function.cpp
    src/function.h
    src/operator.cpp
    src/operator.h
    src/parser.cpp
    src/parser.h
    src/resultvalue.cpp
    src/resultvalue.h
    src/token.cpp
    src/token.h
    src/linenoise.cpp
    src/linenoise.h
    src/numbertype.cpp
    src/numbertype.h
    src/numberhelpers.cpp
    src/numberhelpers.h)

add_executable(termcalc ${SOURCE_FILES} src/termcalc.cpp)

# Tests
set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

find_package(CxxTest)
if(CXXTEST_FOUND)
    add_library(TestLib SHARED ${SOURCE_FILES})

    include_directories(${CXXTEST_INCLUDE_DIR})
    enable_testing()

    CXXTEST_ADD_TEST(tests-general test-general-runner.cpp ${TESTS_DIR}/general.h)
    target_link_libraries(tests-general TestLib)

    CXXTEST_ADD_TEST(tests-int test-int-runner.cpp ${TESTS_DIR}/int.h)
    target_link_libraries(tests-int TestLib)

    CXXTEST_ADD_TEST(tests-float test-float-runner.cpp ${TESTS_DIR}/float.h)
    target_link_libraries(tests-float TestLib)

    CXXTEST_ADD_TEST(tests-complex test-complex-runner.cpp ${TESTS_DIR}/complex.h)
    target_link_libraries(tests-complex TestLib)
endif()