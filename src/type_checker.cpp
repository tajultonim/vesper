#include "type_checker.hpp"
#include <stdexcept>
#include <string>

void requireIntOperands(Type left, Type right,
                        const std::string &operatorSymbol) {
  if (left != Type::INT || right != Type::INT) {
    throw std::runtime_error("Type error: operator '" + operatorSymbol +
                             "' requires int operands");
  }
}

Type TypeChecker::checkExpression(const Expression *expression) {
  if (dynamic_cast<const IntegerExpression *>(expression)) {
    return Type::INT;
  }

  if (dynamic_cast<const BooleanExpression *>(expression)) {
    return Type::BOOL;
  }

  if (auto *binary = dynamic_cast<const BinaryExpression *>(expression)) {
    Type left = checkExpression(binary->left.get());
    Type right = checkExpression(binary->right.get());

    switch (binary->operatorType) {
    case TokenType::PLUS:
      requireIntOperands(left, right, "+");
      return Type::INT;

    case TokenType::MINUS:
      requireIntOperands(left, right, "-");
      return Type::INT;

    case TokenType::STAR:
      requireIntOperands(left, right, "*");
      return Type::INT;

    case TokenType::SLASH:
      requireIntOperands(left, right, "/");
      return Type::INT;

    case TokenType::LESS:
      requireIntOperands(left, right, "<");
      return Type::BOOL;

    case TokenType::LESS_EQUAL:
      requireIntOperands(left, right, "<=");
      return Type::BOOL;

    case TokenType::GREATER:
      requireIntOperands(left, right, ">");
      return Type::BOOL;

    case TokenType::GREATER_EQUAL:
      requireIntOperands(left, right, ">=");
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
      throw std::runtime_error("Unknown binary operator");
    }
  }

  throw std::runtime_error("Unknown expression");
}

void TypeChecker::checkDeclaration(const VariableDeclaration *declaration) {
  Type expressionType = checkExpression(declaration->value.get());

  if (declaration->declaredType.has_value()) {
    if (expressionType != declaration->declaredType.value()) {
      throw std::runtime_error(
          "Type error: declared type does not match initializer");
    }
  }
}
