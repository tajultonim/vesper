cmake_minimum_required(VERSION 3.20)

project(Vesper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(vesper
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/ast.cpp
    src/environment.cpp
    src/interpreter.cpp
    src/type_checker.cpp
    src/token.cpp
)

add_executable(vspfmt
    tools/vspfmt.cpp
    src/lexer.cpp
    src/parser.cpp
    src/ast.cpp
    src/formatter.cpp
    src/token.cpp
)

enable_testing()

add_executable(type_checker_test
    tests/type_checker_test.cpp
    src/type_checker.cpp
    src/ast.cpp
)

add_test(
    NAME TypeChecker
    COMMAND type_checker_test
)