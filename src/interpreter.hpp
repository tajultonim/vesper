#pragma once

#include "ast.hpp"
#include "environment.hpp"
#include <exception>
#include <utility>

class ReturnException : public std::exception {
public:
  Value value;

  explicit ReturnException(Value value) : value(std::move(value)) {}
};

class Interpreter {
public:
  Value evaluate(const Expression *expression);
  Value evaluateCall(const CallExpression *call);
  void execute(const Program &program);
  void executeStatement(const Statement *statement);

private:
  std::shared_ptr<Environment> environment = std::make_shared<Environment>();

  void printValue(const Value &value);
};