#include "token.hpp"

std::string tokenTypeName(TokenType type) {
  switch (type) {

  case TokenType::IMPORT:
    return "IMPORT";
  case TokenType::AS:
    return "AS";
  case TokenType::EXPORT:
    return "EXPORT";
  case TokenType::EXTERN:
    return "EXTERN";

  case TokenType::LET:
    return "LET";
  case TokenType::MUT:
    return "MUT";
  case TokenType::FN:
    return "FN";
  case TokenType::RETURN:
    return "RETURN";

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
  case TokenType::STAR_STAR:
    return "STAR_STAR";
  case TokenType::SLASH_SLASH:
    return "SLASH_SLASH";
  case TokenType::PERCENT:
    return "PERCENT";

  case TokenType::LPAREN:
    return "LPAREN";
  case TokenType::RPAREN:
    return "RPAREN";
  case TokenType::LBRACE:
    return "LBRACE";
  case TokenType::RBRACE:
    return "RBRACE";
  case TokenType::LBRACKET:
    return "LBRACKET";
  case TokenType::RBRACKET:
    return "RBRACKET";

  case TokenType::EQUAL:
    return "EQUAL";
  case TokenType::COMMA:
    return "COMMA";
  case TokenType::SEMICOLON:
    return "SEMICOLON";
  case TokenType::COLON:
    return "COLON";
  case TokenType::DOT:
    return "DOT";

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

void writeToken(std::ostream &output, const Token &token) {
  output << std::left << std::setw(25)
            << tokenTypeName(token.type) + "('" + token.value + "')"
            << token.line << ":" << token.column << '\n';
}

void printToken(const Token &token) { writeToken(std::cerr, token); }

void printTokens(const std::vector<Token> &tokens) {
  writeTokens(std::cerr, tokens);
}

void writeTokens(std::ostream &output, const std::vector<Token> &tokens) {
  for (const auto &token : tokens) {
    writeToken(output, token);
  }
}