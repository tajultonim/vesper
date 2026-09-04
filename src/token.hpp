#pragma once

#include <string>

enum class TokenType {
  LET,
  MUT,
  PRINT,
  IDENTIFIER,

  TYPE,

  INTEGER_LITERAL,
  FLOAT_LITERAL,
  TRUE,
  FALSE,

  PLUS,
  MINUS,
  STAR,
  SLASH,

  LPAREN,
  RPAREN,

  EQUAL_EQUAL,
  NOT_EQUAL,
  LESS,
  LESS_EQUAL,
  GREATER,
  GREATER_EQUAL,

  EQUAL,
  SEMICOLON,
  COLON,
  INVALID,
  END_OF_FILE
};

struct Token {
  TokenType type;
  std::string value;
  int line;
  int column;
};