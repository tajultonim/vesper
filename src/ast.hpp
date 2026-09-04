#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "token.hpp"
#include "type.hpp"

struct Expression {
  int line;
  int column;
  virtual ~Expression() = default;
};

struct IntegerExpression : Expression {
  int value;
};

struct FloatExpression : Expression {
  float value;
};

struct BooleanExpression : Expression {
  bool value;
};

struct StringExpression : Expression {
  std::string value;
};

struct ArrayExpression : Expression {
  std::vector<std::unique_ptr<Expression>> elements;
};

struct BinaryExpression : Expression {
  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;
  TokenType operatorType;
};

struct IdentifierExpression : Expression {
  std::string name;
};

struct Statement {
  virtual ~Statement() = default;
};

struct VariableDeclaration : Statement {
  std::string name;
  std::optional<Type> declaredType;
  std::unique_ptr<Expression> value;
  bool mutable_;
};

struct AssignmentStatement : Statement {
  std::string name;
  std::unique_ptr<Expression> value;
};

struct PrintStatement : Statement {
  std::unique_ptr<Expression> value;
};

struct IfStatement : Statement {
  std::unique_ptr<Expression> condition;
  std::vector<std::unique_ptr<Statement>> thenBranch;
  std::vector<std::unique_ptr<Statement>> elseBranch;
};

struct WhileStatement : Statement {
  std::unique_ptr<Expression> condition;
  std::vector<std::unique_ptr<Statement>> body;
};

struct Program {
  std::vector<std::unique_ptr<Statement>> statements;
};

void printExpression(const Expression *expression, int indent = 0);
