#include <iostream>
#include <stdexcept>

#include "interpreter.hpp"

int Interpreter::evaluate(const Expression *expression) {
  if (auto *integer = dynamic_cast<const IntegerExpression *>(expression)) {
    return integer->value;
  }

  if (auto *identifier =
          dynamic_cast<const IdentifierExpression *>(expression)) {

    return environment.get(identifier->name, identifier->line,
                           identifier->column);
  }

  if (auto *binary = dynamic_cast<const BinaryExpression *>(expression)) {
    int left = evaluate(binary->left.get());
    int right = evaluate(binary->right.get());

    switch (binary->operatorType) {
    case TokenType::PLUS:
      return left + right;

    case TokenType::MINUS:
      return left - right;

    case TokenType::STAR:
      return left * right;

    case TokenType::SLASH:
      return left / right;

    default:
      throw std::runtime_error("Unknown binary operator");
    }
  }

  throw std::runtime_error("Unknown expression");
}

void Interpreter::execute(const Program &program) {
  for (const auto &statement : program.statements) {
    if (auto *let = dynamic_cast<const LetStatement *>(statement.get())) {
      int value = evaluate(let->value.get());
      environment.define(let->name, value);
    } else if (auto *print =
                   dynamic_cast<const PrintStatement *>(statement.get())) {
      int value = evaluate(print->value.get());
      std::cout << value << '\n';
    }
  }
}