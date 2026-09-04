#include "token.hpp"

std::string tokenTypeName(TokenType type) {
  switch (type) {

  case TokenType::LET:
    return "LET";
  case TokenType::MUT:
    return "MUT";

  case TokenType::IF:
    return "IF";
  case TokenType::ELSE:
    return "ELSE";

  case TokenType::WHILE:
    return "WHILE";
  case TokenType::BREAK:
    return "BREAK";
  case TokenType::FOR:
    return "FOR";

  case TokenType::PRINT:
    return "PRINT";

  case TokenType::TYPE:
    return "TYPE";
  case TokenType::INTEGER_LITERAL:
    return "INTEGER_LITERAL";
  case TokenType::FLOAT_LITERAL:
    return "FLOAT_LITERAL";
  case TokenType::STRING_LITERAL:
    return "STRING_LITERAL";
  case TokenType::TRUE:
    return "TRUE";
  case TokenType::FALSE:
    return "FALSE";

  case TokenType::IDENTIFIER:
    return "IDENTIFIER";

  case TokenType::PLUS:
    return "PLUS";
  case TokenType::MINUS:
    return "MINUS";
  case TokenType::STAR:
    return "STAR";
  case TokenType::SLASH:
    return "SLASH";

  case TokenType::LPAREN:
    return "LPAREN";
  case TokenType::RPAREN:
    return "RPAREN";
  case TokenType::LBRACE:
    return "LBRACE";
  case TokenType::RBRACE:
    return "RBRACE";

  case TokenType::EQUAL:
    return "EQUAL";
  case TokenType::SEMICOLON:
    return "SEMICOLON";
  case TokenType::COLON:
    return "COLON";

  case TokenType::INVALID:
    return "INVALID";
  case TokenType::END_OF_FILE:
    return "END_OF_FILE";

  case TokenType::EQUAL_EQUAL:
    return "EQUAL_EQUAL";
  case TokenType::NOT_EQUAL:
    return "NOT_EQUAL";
  case TokenType::LESS:
    return "LESS";
  case TokenType::LESS_EQUAL:
    return "LESS_EQUAL";
  case TokenType::GREATER:
    return "GREATER";
  case TokenType::GREATER_EQUAL:
    return "GREATER_EQUAL";
  }

  return "unknown";
}

void printToken(const Token &token) {
  std::cout << std::left << std::setw(25)
            << tokenTypeName(token.type) + "('" + token.value + "')"
            << token.line << ":" << token.column << '\n';
}

void printTokens(const std::vector<Token> &tokens) {
  for (const auto &token : tokens) {
    printToken(token);
  }
}