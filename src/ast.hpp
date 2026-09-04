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

struct BooleanExpression : Expression {
  bool value;
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

struct LetStatement : Statement {
  std::string name;
  std::unique_ptr<Expression> value;
  std::optional<Type> declaredType;
  bool mutable_;
};

struct PrintStatement : Statement {
  std::unique_ptr<Expression> value;
};

struct Program {
  std::vector<std::unique_ptr<Statement>> statements;
};

void printExpression(const Expression *expression, int indent = 0);
