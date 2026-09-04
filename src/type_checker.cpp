#include "type_checker.hpp"
#include <stdexcept>
#include <string>

bool isNumeric(Type type) { return type == Type::INT || type == Type::FLOAT; }

void requireNumericOperands(Type left, Type right,
                            const std::string &operatorSymbol) {
  if (!isNumeric(left) || !isNumeric(right)) {
    throw std::runtime_error("Type error: operator '" + operatorSymbol +
                             "' requires numeric operands");
  }
}

Type TypeChecker::checkExpression(const Expression *expression) {
  if (dynamic_cast<const IntegerExpression *>(expression)) {
    return Type::INT;
  }

  if (dynamic_cast<const StringExpression *>(expression)) {
    return Type::STRING;
  }

  if (dynamic_cast<const FloatExpression *>(expression)) {
    return Type::FLOAT;
  }

  if (dynamic_cast<const BooleanExpression *>(expression)) {
    return Type::BOOL;
  }

  if (auto *identifier =
          dynamic_cast<const IdentifierExpression *>(expression)) {
    auto it = types.find(identifier->name);

    if (it == types.end()) {
      throw std::runtime_error("Undefined variable '" + identifier->name + "'");
    }

    return it->second.type;
  }

  if (auto *binary = dynamic_cast<const BinaryExpression *>(expression)) {
    Type left = checkExpression(binary->left.get());
    Type right = checkExpression(binary->right.get());

    switch (binary->operatorType) {
    case TokenType::PLUS:
      if (left == Type::STRING && right == Type::STRING) {
        return Type::STRING;
      }
      requireNumericOperands(left, right, "+");
      if (left == Type::FLOAT || right == Type::FLOAT)
        return Type::FLOAT;

      return Type::INT;

    case TokenType::MINUS:
      requireNumericOperands(left, right, "-");
      if (left == Type::FLOAT || right == Type::FLOAT)
        return Type::FLOAT;

      return Type::INT;

    case TokenType::STAR:
      if (left == Type::STRING && right == Type::INT) {
        return Type::STRING;
      }

      requireNumericOperands(left, right, "*");
      if (left == Type::FLOAT || right == Type::FLOAT)
        return Type::FLOAT;

      return Type::INT;

    case TokenType::SLASH:
      requireNumericOperands(left, right, "/");
      if (left == Type::FLOAT || right == Type::FLOAT)
        return Type::FLOAT;

      return Type::INT;

    case TokenType::LESS:
      requireNumericOperands(left, right, "<");
      return Type::BOOL;

    case TokenType::LESS_EQUAL:
      requireNumericOperands(left, right, "<=");
      return Type::BOOL;

    case TokenType::GREATER:
      requireNumericOperands(left, right, ">");
      return Type::BOOL;

    case TokenType::GREATER_EQUAL:
      requireNumericOperands(left, right, ">=");
      return Type::BOOL;

    case TokenType::EQUAL_EQUAL:
      if (left != right)
        throw std::runtime_error(
            "Type error: operands of '==' must have the same type");

      return Type::BOOL;

    case TokenType::NOT_EQUAL:
      if (left != right)
        throw std::runtime_error(
            "Type error: operands of '!=' must have the same type");

      return Type::BOOL;

    default:
      throw std::runtime_error("Type error: Unknown binary operator");
    }
  }

  throw std::runtime_error("Type error: Unknown expression");
}

void TypeChecker::checkAssignment(const AssignmentStatement *assignment) {
  auto it = types.find(assignment->name);

  if (it == types.end()) {
    throw std::runtime_error("Undefined variable '" + assignment->name + "'");
  }

  Type valueType = checkExpression(assignment->value.get());

  if (!it->second.mutable_) {
    throw std::runtime_error("Cannot assign to immutable variable '" +
                             assignment->name + "'");
  }

  if (it->second.type != valueType) {
    throw std::runtime_error(
        "Type error: cannot assign value of different type");
  }
}

void TypeChecker::checkDeclaration(const VariableDeclaration *declaration) {
  Type expressionType = checkExpression(declaration->value.get());

  if (declaration->declaredType.has_value()) {
    if (expressionType != declaration->declaredType.value()) {
      throw std::runtime_error(
          "Type error: declared type does not match initializer");
    }
  }

  types[declaration->name] =
      VariableInfo{expressionType, declaration->mutable_};
}

void TypeChecker::checkStatement(const Statement *statement) {
  if (auto *declaration =
          dynamic_cast<const VariableDeclaration *>(statement)) {
    checkDeclaration(declaration);
  } else if (auto *assignment =
                 dynamic_cast<const AssignmentStatement *>(statement)) {
    checkAssignment(assignment);
  } else if (auto *print = dynamic_cast<const PrintStatement *>(statement)) {
    checkExpression(print->value.get());
  } else if (auto *ifStatement = dynamic_cast<const IfStatement *>(statement)) {
    checkIfStatement(ifStatement);
  } else if (auto *whileStatement =
                 dynamic_cast<const WhileStatement *>(statement)) {
    checkWhileStatement(whileStatement);
  } else {
    throw std::runtime_error("Unknown statement");
  }
}

void TypeChecker::checkIfStatement(const IfStatement *ifStatement) {
  Type conditionType = checkExpression(ifStatement->condition.get());

  if (conditionType != Type::BOOL) {
    throw std::runtime_error(
        "Type error: condition of 'if' statement must be a boolean");
  }

  for (const auto &statement : ifStatement->thenBranch) {
    checkStatement(statement.get());
  }

  for (const auto &statement : ifStatement->elseBranch) {
    checkStatement(statement.get());
  }
}

void TypeChecker::checkWhileStatement(const WhileStatement *whileStatement) {
  Type conditionType = checkExpression(whileStatement->condition.get());

  if (conditionType != Type::BOOL) {
    throw std::runtime_error(
        "Type error: condition of 'while' statement must be a boolean");
  }

  for (const auto &statement : whileStatement->body) {
    checkStatement(statement.get());
  }
}

void TypeChecker::checkProgram(const Program &program) {
  for (const auto &statement : program.statements) {
    checkStatement(statement.get());
  }
}