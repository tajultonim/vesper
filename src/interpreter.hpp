#pragma once

#include "ast.hpp"
#include "environment.hpp"

class Interpreter {
public:
  Value evaluate(const Expression *expression);
  void execute(const Program &program);

private:
  Environment environment;
};