#pragma once
#include <memory>

#include "ast.hpp"
#include "token.hpp"
#include <vector>

class Parser {

public:
  Parser(const std::vector<Token> &tokens);

  Token current() const;
  Token peek() const;
  void advance();
  bool expect(TokenType type);

  std::unique_ptr<Expression> parsePrimary();
  std::unique_ptr<Expression> parseExpression();
  std::unique_ptr<Expression> parseComparism();
  std::unique_ptr<Expression> parseAddition();
  std::unique_ptr<Expression> parseMultiplication();
  std::unique_ptr<Expression> parseArray();
  std::unique_ptr<Expression> parsePostfix();
  std::unique_ptr<Expression> parseExponentiation();
  std::unique_ptr<Expression> parseUnary();
  Type parseType();
  Program parseProgram();

  std::unique_ptr<Statement> parseDeclaration();
  std::unique_ptr<Statement> parseAssignment();
  std::unique_ptr<Statement> parseIfStatement();
  std::unique_ptr<Statement> parseWhileStatement();
  std::unique_ptr<Statement> parsePrint();
  std::unique_ptr<Statement> parseStatement();
  std::unique_ptr<Statement> parseFunction();
  std::unique_ptr<Statement> parseExpressionStatement();
  std::unique_ptr<Expression> parseCall(std::unique_ptr<Expression> callee);
  std::unique_ptr<Statement> parseReturn();

private:
  std::vector<Token> tokens;
  std::size_t position = 0;
  std::unique_ptr<Statement> handleIdentifier();
};

std::string tokenTypeName(TokenType type);