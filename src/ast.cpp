#include "ast.hpp"
#include <iostream>

void printExpression(const Expression *expression, int indent) {
  if (expression == nullptr) {
    std::cout << std::string(indent, ' ') << "null\n";
    return;
  }

  if (auto *integer = dynamic_cast<const IntegerExpression *>(expression)) {
    std::cout << std::string(indent, ' ') << integer->value << '\n';

    return;
  }

  if (auto *identifier =
          dynamic_cast<const IdentifierExpression *>(expression)) {
    std::cout << std::string(indent, ' ') << identifier->name << '\n';

    return;
  }

  if (auto *binary = dynamic_cast<const BinaryExpression *>(expression)) {
    std::string op;

    switch (binary->operatorType) {
    case TokenType::PLUS:
      op = "+";
      break;

    case TokenType::MINUS:
      op = "-";
      break;

    case TokenType::STAR:
      op = "*";
      break;

    case TokenType::SLASH:
      op = "/";
      break;

    case TokenType::EQUAL_EQUAL:
      op = "==";
      break;

    case TokenType::NOT_EQUAL:
      op = "!=";
      break;

    case TokenType::LESS:
      op = "<";
      break;

    case TokenType::LESS_EQUAL:
      op = "<=";
      break;

    case TokenType::GREATER:
      op = ">";
      break;

    case TokenType::GREATER_EQUAL:
      op = ">=";
      break;

    default:
      op = "?";
      break;
    }

    std::cout << std::string(indent, ' ') << op << '\n';

    printExpression(binary->left.get(), indent + 2);
    printExpression(binary->right.get(), indent + 2);

    return;
  }

  std::cout << std::string(indent, ' ') << "unknown expression\n";
}
