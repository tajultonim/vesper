#pragma once

#include <string>

enum class TokenType {
  LET,
  PRINT,
  INTEGER,
  IDENTIFIER,

  PLUS,
  MINUS,
  STAR,
  SLASH,

  LPAREN,
  RPAREN,

  TRUE,
  FALSE,
  EQUAL_EQUAL,
  NOT_EQUAL,
  LESS,
  LESS_EQUAL,
  GREATER,
  GREATER_EQUAL,

  EQUAL,
  SEMICOLON,
  INVALID,
  END_OF_FILE
};

struct Token {
  TokenType type;
  std::string value;
  int line;
  int column;
};