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
    src/termcalc.cpp
    src/token.cpp
    src/token.h)

add_executable(termcalc ${SOURCE_FILES})