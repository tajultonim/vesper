#pragma once

#include <string>
#include <vector>

#include "token.hpp"

class Lexer {

public:
  Lexer(const std::string &source);
  std::vector<Token> tokenize();

private:
  std::string source;
  std::size_t position = 0;
  int line = 1;
  int column = 1;
  char current() const;
  char peek() const;
  void advance();
  Token readNumber();
  Token readString();
  Token readComment();
  Token readIdentifier();
  void skipWhitespace();
};