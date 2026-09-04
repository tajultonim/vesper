#pragma once

#include <string>
#include <unordered_map>

#include "ast.hpp"
#include "type.hpp"

struct VariableInfo {
  Type type;
  bool mutable_;
};

class TypeChecker {
public:
  Type checkExpression(const Expression *expression);
  void checkDeclaration(const VariableDeclaration *declaration);
  void checkAssignment(const AssignmentStatement *assignment);

private:
  std::unordered_map<std::string, VariableInfo> types;
};