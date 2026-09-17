#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "ast.hpp"

enum class SymbolKind { VARIABLE, FUNCTION, PARAMETER, MODULE };

struct Symbol {
  std::string name;
  SymbolKind kind;
};

class Resolver {
public:
  void resolveProgram(const Program &program);

private:
  void resolveStatement(const Statement *statement);

  void resolveDeclaration(const VariableDeclaration *declaration);

  void resolveAssignment(const AssignmentStatement *assignment);

  void resolveFunction(const FunctionStatement *function);

  void resolveReturn(const ReturnStatement *returnStatement);

  void resolveIf(const IfStatement *ifStatement);

  void resolveWhile(const WhileStatement *whileStatement);

  void resolveImport(const ImportStatement *importStatement);

  void resolveExpression(const Expression *expression);

  void resolveIdentifier(const IdentifierExpression *identifier);

  void resolveMember(const MemberExpression *member);

  void resolveCall(const CallExpression *call);

  // Scope management
  void enterScope();
  void leaveScope();

  void declare(const std::string &name, SymbolKind kind);

  const Symbol *lookup(const std::string &name) const;

  using Scope = std::unordered_map<std::string, Symbol>;

  std::vector<Scope> scopes;
};