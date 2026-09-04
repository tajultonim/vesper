#include <cassert>
#include <iostream>

#include "../src/type_checker.hpp"
#include "../src/ast.hpp"

int main()
{
    Program program;

    auto declaration = std::make_unique<VariableDeclaration>();

    declaration->name = "x";
    declaration->mutable_ = false;

    auto integer = std::make_unique<IntegerExpression>();
    integer->value = 42;

    declaration->value = std::move(integer);

    program.statements.push_back(std::move(declaration));

    TypeChecker checker;

    checker.checkProgram(program);

    std::cout << "Test passed!\n";

    return 0;
}