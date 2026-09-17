#pragma once

#include "ast.hpp"
#include "environment.hpp"
#include "module.hpp"
#include <exception>
#include <unordered_map>
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
  void registerModule(const std::string &alias,
                      const std::shared_ptr<Module> &module);
  void execute(const Program &program);
  void executeStatement(const Statement *statement);

private:
  std::shared_ptr<Environment> environment = std::make_shared<Environment>();
  std::unordered_map<std::string, std::shared_ptr<Module>> importedModules;
  std::unordered_map<std::string, std::shared_ptr<Function>> importedFunctions;

  void printValue(const Value &value);
};