#pragma once

#include "ast.hpp"
#include "environment.hpp"

class Interpreter
{
public:
    int evaluate(const Expression* expression);
    void execute(const Program& program);

private:
    Environment environment;
};