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

struct IndexExpression : Expression {
  std::unique_ptr<Expression> object;
  std::unique_ptr<Expression> index;
};

struct UnaryExpression : Expression {
  TokenType operatorType;
  std::unique_ptr<Expression> operand;
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

struct ExpressionStatement : Statement {
  std::unique_ptr<Expression> expression;
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
  std::vector<std::unique_ptr<Expression>> values;
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

struct FunctionStatement : Statement {
  struct Parameter {
    std::string name;
    Type type;
    std::unique_ptr<Expression> defaultValue;

    Parameter(std::string name, Type type,
              std::unique_ptr<Expression> defaultValue = nullptr)
        : name(std::move(name)), type(std::move(type)),
          defaultValue(std::move(defaultValue)) {}
  };

  std::string name;
  std::vector<Parameter> parameters;
  Type returnType;
  std::vector<std::unique_ptr<Statement>> body;

  FunctionStatement(std::string name, std::vector<Parameter> parameters,
                    Type returnType)
      : name(std::move(name)), parameters(std::move(parameters)),
        returnType(std::move(returnType)) {}
};

struct ReturnStatement : Statement {
  std::unique_ptr<Expression> value;
};

struct CallExpression : Expression {
  std::unique_ptr<Expression> callee;
  std::vector<std::unique_ptr<Expression>> arguments;
};

struct Program {
  std::vector<std::unique_ptr<Statement>> statements;
};

void printExpression(const Expression *expression, int indent = 0);
