#pragma once

#include "ast.hpp"
#include "type.hpp"

class TypeChecker {
public:
  Type checkExpression(const Expression *expression);
  void checkDeclaration(const VariableDeclaration *declaration);
};