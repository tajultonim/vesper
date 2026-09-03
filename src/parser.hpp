#pragma once
#include <memory>

#include "ast.hpp"
#include "token.hpp"
#include <vector>

class Parser {

public:
  Parser(const std::vector<Token> &tokens);

  Token current() const;
  void advance();
  bool expect(TokenType type);

  std::unique_ptr<Expression> parsePrimary();
  std::unique_ptr<Expression> parseExpression();
  std::unique_ptr<Expression> parseMultiplication();
  Program parseProgram();

  std::unique_ptr<Statement> parseLet();
  std::unique_ptr<Statement> parsePrint();

private:
  std::vector<Token> tokens;
  std::size_t position = 0;
};

