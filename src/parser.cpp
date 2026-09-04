#include "parser.hpp"
#include <iostream>

Parser::Parser(const std::vector<Token> &input) : tokens(input) {}

std::unique_ptr<Statement> Parser::parseLet() {
  auto statement = std::make_unique<LetStatement>();

  expect(TokenType::LET);

  statement->name = current().value;
  expect(TokenType::IDENTIFIER);

  expect(TokenType::EQUAL);

  statement->value = parseExpression();

  expect(TokenType::SEMICOLON);

  return statement;
}

Token Parser::current() const { return tokens[position]; }

std::string tokenTypeName(TokenType type) {
  switch (type) {
  case TokenType::LET:
    return "let";
  case TokenType::PRINT:
    return "print";

  case TokenType::INTEGER:
    return "integer";

  case TokenType::IDENTIFIER:
    return "identifier";

  case TokenType::PLUS:
    return "+";

  case TokenType::MINUS:
    return "-";

  case TokenType::STAR:
    return "*";

  case TokenType::SLASH:
    return "/";

  case TokenType::LPAREN:
    return "(";

  case TokenType::RPAREN:
    return ")";

  case TokenType::EQUAL:
    return "=";

  case TokenType::SEMICOLON:
    return ";";

  case TokenType::INVALID:
    return "invalid";

  case TokenType::END_OF_FILE:
    return "end of file";

  case TokenType::TRUE:
    return "true";

  case TokenType::FALSE:
    return "false";

  case TokenType::EQUAL_EQUAL:
    return "==";
  case TokenType::NOT_EQUAL:
    return "!=";
  case TokenType::LESS:
    return "<";
  case TokenType::LESS_EQUAL:
    return "<=";
  case TokenType::GREATER:
    return ">";
  case TokenType::GREATER_EQUAL:
    return ">=";
  }

  return "unknown";
}

void Parser::advance() {
  if (position < tokens.size()) {
    position++;
  }
}

bool Parser::expect(TokenType type) {
  if (current().type != type) {
    std::cerr << "Parser error at line " << current().line << ", column "
              << current().column << ": expected " << tokenTypeName(type)
              << ", got " << tokenTypeName(current().type) << '\n';

    return false;
  }

  advance();
  return true;
}

std::unique_ptr<Expression> Parser::parseExpression() {
  return parseComparism();
}

std::unique_ptr<Expression> Parser::parseComparism() {
  auto left = parseAddition();

  while (current().type == TokenType::EQUAL_EQUAL ||
         current().type == TokenType::NOT_EQUAL ||
         current().type == TokenType::LESS ||
         current().type == TokenType::LESS_EQUAL ||
         current().type == TokenType::GREATER ||
         current().type == TokenType::GREATER_EQUAL) {
    TokenType op = current().type;
    advance();

    auto right = parseAddition();

    auto expression = std::make_unique<BinaryExpression>();

    expression->left = std::move(left);
    expression->right = std::move(right);
    expression->operatorType = op;

    left = std::move(expression);
  }

  return left;
}

std::unique_ptr<Expression> Parser::parseAddition() {
  auto left = parseMultiplication();

  while (current().type == TokenType::PLUS ||
         current().type == TokenType::MINUS) {
    TokenType op = current().type;
    advance();

    auto right = parseMultiplication();

    auto expression = std::make_unique<BinaryExpression>();

    expression->left = std::move(left);
    expression->right = std::move(right);
    expression->operatorType = op;

    left = std::move(expression);
  }

  return left;
}

std::unique_ptr<Expression> Parser::parsePrimary() {
  if (current().type == TokenType::LPAREN) {
    advance();

    auto expression = parseExpression();

    expect(TokenType::RPAREN);

    return expression;
  }

  if (current().type == TokenType::INTEGER) {
    auto expression = std::make_unique<IntegerExpression>();
    expression->value = std::stoi(current().value);

    advance();
    return expression;
  }

  if (current().type == TokenType::IDENTIFIER) {
    auto expression = std::make_unique<IdentifierExpression>();
    expression->name = current().value;
    expression->line = current().line;
    expression->column = current().column;
    advance();
    return expression;
  }

  if (current().type == TokenType::TRUE || current().type == TokenType::FALSE) {
    auto expression = std::make_unique<BooleanExpression>();
    expression->value = (current().type == TokenType::TRUE);
    expression->line = current().line;
    expression->column = current().column;
    advance();
    return expression;
  }

  return nullptr;
}

std::unique_ptr<Expression> Parser::parseMultiplication() {
  auto left = parsePrimary();

  while (current().type == TokenType::STAR ||
         current().type == TokenType::SLASH) {
    TokenType op = current().type;
    advance();

    auto right = parsePrimary();

    auto expression = std::make_unique<BinaryExpression>();

    expression->left = std::move(left);
    expression->right = std::move(right);
    expression->operatorType = op;

    left = std::move(expression);
  }

  return left;
}

Program Parser::parseProgram() {
  Program program;

  while (current().type != TokenType::END_OF_FILE) {
    if (current().type == TokenType::LET) {
      program.statements.push_back(parseLet());
    } else if (current().type == TokenType::PRINT) {
      program.statements.push_back(parsePrint());
    } else {
      std::cerr << "Unexpected token\n";
      break;
    }
  }

  return program;
}

std::unique_ptr<Statement> Parser::parsePrint() {
  auto statement = std::make_unique<PrintStatement>();

  expect(TokenType::PRINT);
  expect(TokenType::LPAREN);

  statement->value = parseExpression();

  expect(TokenType::RPAREN);
  expect(TokenType::SEMICOLON);

  return statement;
}
