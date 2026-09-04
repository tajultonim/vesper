#pragma once

#include <string>
#include <unordered_map>

#include "ast.hpp"
#include "type.hpp"

class TypeChecker {
public:
  Type checkExpression(const Expression *expression);
  void checkDeclaration(const VariableDeclaration *declaration);

private:
  std::unordered_map<std::string, Type> types;
};