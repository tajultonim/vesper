#include <iostream>
#include <stdexcept>

#include "interpreter.hpp"

std::string valueTypeName(const Value &value) {
  if (std::holds_alternative<int>(value))
    return "Int";

  if (std::holds_alternative<bool>(value))
    return "Bool";

  return "Unknown";
}

void requireIntOperands(const Value &left, const Value &right,
                        const std::string &operatorSymbol) {
  if (!std::holds_alternative<int>(left) ||
      !std::holds_alternative<int>(right)) {
    throw std::runtime_error("Type error: operator '" + operatorSymbol +
                             "' cannot be applied to " + valueTypeName(left) +
                             " and " + valueTypeName(right));
  }
}

Value Interpreter::evaluate(const Expression *expression) {
  if (auto *integer = dynamic_cast<const IntegerExpression *>(expression)) {
    return integer->value;
  }

  if (auto *identifier =
          dynamic_cast<const IdentifierExpression *>(expression)) {

    return environment.get(identifier->name, identifier->line,
                           identifier->column);
  }

  if (auto *binary = dynamic_cast<const BinaryExpression *>(expression)) {
    auto left = evaluate(binary->left.get());
    auto right = evaluate(binary->right.get());

    switch (binary->operatorType) {
    case TokenType::PLUS: {
      requireIntOperands(left, right, "+");
      int leftInt = std::get<int>(left);
      int rightInt = std::get<int>(right);

      return leftInt + rightInt;
    }
    case TokenType::MINUS: {
      requireIntOperands(left, right, "-");

      int leftInt = std::get<int>(left);
      int rightInt = std::get<int>(right);
      return leftInt - rightInt;
    }

    case TokenType::STAR: {
      requireIntOperands(left, right, "*");
      int leftInt = std::get<int>(left);
      int rightInt = std::get<int>(right);
      return leftInt * rightInt;
    }

    case TokenType::SLASH: {
      requireIntOperands(left, right, "+");
      int leftInt = std::get<int>(left);
      int rightInt = std::get<int>(right);

      if (rightInt == 0) {
        throw std::runtime_error("Division by zero");
      }

      return leftInt / rightInt;
    }

    case TokenType::EQUAL_EQUAL:
      if (valueTypeName(left) != valueTypeName(right)) {
        throw std::runtime_error(
            "Type error: operator '==' cannot be applied to " +
            valueTypeName(left) + " and " + valueTypeName(right));
      }
      return left == right;

    case TokenType::NOT_EQUAL:
      if (valueTypeName(left) != valueTypeName(right)) {
        throw std::runtime_error(
            "Type error: operator '!=' cannot be applied to " +
            valueTypeName(left) + " and " + valueTypeName(right));
      }
      return left != right;

    case TokenType::LESS:
      requireIntOperands(left, right, "<");
      return std::get<int>(left) < std::get<int>(right);

    case TokenType::LESS_EQUAL:
      requireIntOperands(left, right, "<=");
      return std::get<int>(left) <= std::get<int>(right);

    case TokenType::GREATER:
      requireIntOperands(left, right, ">");
      return std::get<int>(left) > std::get<int>(right);

    case TokenType::GREATER_EQUAL:
      requireIntOperands(left, right, ">=");
      return std::get<int>(left) >= std::get<int>(right);

    default:
      throw std::runtime_error("Unknown binary operator");
    }
  }

  if (auto *integer = dynamic_cast<const IntegerExpression *>(expression)) {
    return integer->value;
  }

  if (auto *boolean = dynamic_cast<const BooleanExpression *>(expression)) {
    return boolean->value;
  }

  throw std::runtime_error("Unknown expression");
}

void Interpreter::execute(const Program &program) {
  for (const auto &statement : program.statements) {
    if (auto *let = dynamic_cast<const LetStatement *>(statement.get())) {
      Value value = evaluate(let->value.get());
      environment.define(let->name, value);
    } else if (auto *print =
                   dynamic_cast<const PrintStatement *>(statement.get())) {
      Value value = evaluate(print->value.get());
      std::visit([](auto &&value) { std::cout << value << '\n'; }, value);
    }
  }
}
