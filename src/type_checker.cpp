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
