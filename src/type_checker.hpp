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
  void checkProgram(const Program &program);

private:
  Type checkExpression(const Expression *expression);
  void checkStatement(const Statement *statement);
  void checkDeclaration(const VariableDeclaration *declaration);
  void checkAssignment(const AssignmentStatement *assignment);
  void checkIfStatement(const IfStatement *ifStatement);
  void checkWhileStatement(const WhileStatement *whileStatement);

  std::unordered_map<std::string, VariableInfo> types;
};