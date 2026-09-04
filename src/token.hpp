#pragma once

#include <string>
#include <iomanip>
#include <iostream>
#include <vector>

enum class TokenType {
  LET,
  MUT,
  PRINT,
  IDENTIFIER,

  TYPE,

  INTEGER_LITERAL,
  FLOAT_LITERAL,
  STRING_LITERAL,
  TRUE,
  FALSE,

  PLUS,
  MINUS,
  STAR,
  SLASH,

  LPAREN,
  RPAREN,
  LBRACE,
  RBRACE,

  EQUAL_EQUAL,
  NOT_EQUAL,
  LESS,
  LESS_EQUAL,
  GREATER,
  GREATER_EQUAL,

  IF,
  ELSE,
  WHILE,
  BREAK,
  FOR,

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


std::string tokenTypeName(TokenType type);
void printToken(const Token& token);
void printTokens(const std::vector<Token> &tokens);