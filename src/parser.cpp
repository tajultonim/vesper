#include "parser.hpp"
#include "token.hpp"
#include <iostream>

Parser::Parser(const std::vector<Token> &input) : tokens(input) {}

Token Parser::current() const { return tokens[position]; }
Token Parser::peek() const { return tokens[position + 1]; }

void Parser::advance() {

  if (position < tokens.size()) {
    position++;
    // if (current().type == TokenType::COMMENT) {
    //   advance();
    // }
  }
}

bool Parser::expect(TokenType type) {
  if (current().type != type) {
    std::cerr << "PARSER ERROR: Parser error at line " << current().line
              << ", column " << current().column << ": expected "
              << tokenTypeName(type) << ", got "
              << tokenTypeName(current().type) << '\n';

    return false;
  }
  advance();
  return true;
}

Type Parser::parseType() {
  if (current().type == TokenType::TYPE) {
    std::string typeName = current().value;
    advance();
    if (typeName == "int")
      return Type(Type::Kind::INT);

    if (typeName == "float")
      return Type(Type::Kind::FLOAT);

    if (typeName == "bool")
      return Type(Type::Kind::BOOL);

    if (typeName == "string")
      return Type(Type::Kind::STRING);

    throw std::runtime_error("PARSER ERROR:Unknown type: " + typeName);
  }

  if (current().type == TokenType::LBRACKET) {
    advance();

    Type elementType = parseType();

    if (current().type != TokenType::RBRACKET) {
      throw std::runtime_error("PARSER ERROR:Expected ']'");
    }

    advance();

    return Type(Type::Kind::ARRAY,
                std::make_unique<Type>(std::move(elementType)));
  }

  throw std::runtime_error("PARSER ERROR: Expected type got " +
                           tokenTypeName(current().type) + "at line " +
                           std::to_string(current().line) + ", column " +
                           std::to_string(current().column));
}

std::unique_ptr<Statement> Parser::parseDeclaration() {
  auto statement = std::make_unique<VariableDeclaration>();
  statement->line = current().line;
  statement->column = current().column;
  if (current().type == TokenType::MUT) {
    statement->mutable_ = true;
    advance();
  } else if (current().type == TokenType::LET) {
    statement->mutable_ = false;
    advance();
  } else {
    std::cerr << "PARSER ERROR: Parser error at line " << current().line
              << ", column " << current().column
              << ": expected 'let' or 'mut', got "
              << tokenTypeName(current().type) << '\n';
    return nullptr;
  }

  statement->name = current().value;
  expect(TokenType::IDENTIFIER);

  if (current().type == TokenType::COLON) {
    advance();
    statement->declaredType = parseType();
  }

  expect(TokenType::EQUAL);

  statement->value = parseExpression();
  expect(TokenType::SEMICOLON);

  return statement;
}

std::unique_ptr<Statement> Parser::parseAssignment() {
  auto statement = std::make_unique<AssignmentStatement>();
  statement->line = current().line;
  statement->column = current().column;
  statement->name = current().value;
  expect(TokenType::IDENTIFIER);

  expect(TokenType::EQUAL);

  statement->value = parseExpression();

  expect(TokenType::SEMICOLON);

  return statement;
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

std::unique_ptr<Expression> Parser::parseMultiplication() {
  auto left = parseUnary();
  ;

  while (current().type == TokenType::STAR ||
         current().type == TokenType::SLASH ||
         current().type == TokenType::SLASH_SLASH ||
         current().type == TokenType::PERCENT) {
    TokenType op = current().type;
    advance();

    auto right = parseUnary();

    auto expression = std::make_unique<BinaryExpression>();

    expression->left = std::move(left);
    expression->right = std::move(right);
    expression->operatorType = op;

    left = std::move(expression);
  }

  return left;
}

std::unique_ptr<Expression> Parser::parseExponentiation() {
  auto left = parsePostfix();

  if (current().type != TokenType::STAR_STAR)
    return left;

  TokenType op = current().type;
  advance();

  auto right = parseUnary();

  auto expression = std::make_unique<BinaryExpression>();

  expression->left = std::move(left);
  expression->right = std::move(right);
  expression->operatorType = op;

  return expression;
}

std::unique_ptr<Expression> Parser::parseUnary() {
  if (current().type == TokenType::MINUS || current().type == TokenType::PLUS) {
    TokenType op = current().type;
    advance();

    auto operand = parseUnary();

    auto expression = std::make_unique<UnaryExpression>();

    expression->operatorType = op;
    expression->operand = std::move(operand);

    return expression;
  }

  return parseExponentiation();
}

std::unique_ptr<Expression> Parser::parsePrimary() {

  if (current().type == TokenType::LBRACKET) {
    return parseArray();
  }

  if (current().type == TokenType::STRING_LITERAL) {
    auto expression = std::make_unique<StringExpression>();
    expression->value = current().value;
    expression->line = current().line;
    expression->column = current().column;
    advance();
    return expression;
  }

  if (current().type == TokenType::LPAREN) {
    advance();

    auto expression = parseExpression();

    expect(TokenType::RPAREN);

    return expression;
  }

  if (current().type == TokenType::INTEGER_LITERAL) {
    auto expression = std::make_unique<IntegerExpression>();
    expression->value = std::stoi(current().value);

    advance();
    return expression;
  }

  if (current().type == TokenType::FLOAT_LITERAL) {
    auto expression = std::make_unique<FloatExpression>();
    expression->value = std::stof(current().value);

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

std::unique_ptr<Statement> Parser::parseStatement() {
  switch (current().type) {
  case TokenType::LET:
  case TokenType::MUT:
    return parseDeclaration();

  case TokenType::FN:
    return parseFunction();
  case TokenType::RETURN:
    return parseReturn();

  case TokenType::PRINT:
    return parsePrint();

  case TokenType::IF:
    return parseIfStatement();

  case TokenType::COMMENT:
    advance();
    return parseStatement();

  case TokenType::IDENTIFIER:
    return handleIdentifier();

  case TokenType::WHILE:
    return parseWhileStatement();

  case TokenType::STRING_LITERAL:
  case TokenType::INTEGER_LITERAL:
  case TokenType::FLOAT_LITERAL:
  case TokenType::TRUE:
  case TokenType::FALSE:
    return parseExpressionStatement();

  default:
    std::cout << tokenTypeName(current().type) << std::endl;
    throw std::runtime_error("PARSER ERROR: Unexpected statement at line " +
                             std::to_string(current().line) + ", column " +
                             std::to_string(current().column));
  }
}

std::unique_ptr<Statement> Parser::handleIdentifier() {
  if (peek().type == TokenType::EQUAL)
    return parseAssignment();

  // Parse the identifier first
  auto callee = std::make_unique<IdentifierExpression>();

  callee->name = current().value;
  callee->line = current().line;
  callee->column = current().column;

  advance();

  // Function call
  if (current().type == TokenType::LPAREN) {
    auto statement = std::make_unique<ExpressionStatement>();
    statement->line = current().line;

    statement->column = current().column;

    statement->expression = parseCall(std::move(callee));

    expect(TokenType::SEMICOLON);

    return statement;
  }

  throw std::runtime_error(
      "PARSER ERROR: Unexpected token after identifier at line " +
      std::to_string(current().line) + ", column " +
      std::to_string(current().column));
}

std::unique_ptr<Expression>
Parser::parseCall(std::unique_ptr<Expression> callee) {
  auto call = std::make_unique<CallExpression>();

  call->callee = std::move(callee);

  expect(TokenType::LPAREN);

  // No arguments
  if (current().type == TokenType::RPAREN) {
    advance();
    return call;
  }

  // Arguments
  while (true) {
    call->arguments.push_back(parseExpression());

    if (current().type != TokenType::COMMA)
      break;

    advance(); // consume ','
  }

  expect(TokenType::RPAREN);

  return call;
}

std::unique_ptr<Statement> Parser::parseExpressionStatement() {
  auto statement = std::make_unique<ExpressionStatement>();
  statement->line = current().line;
  statement->column = current().column;

  statement->expression = parseExpression();
  expect(TokenType::SEMICOLON);
  return statement;
}

std::unique_ptr<Statement> Parser::parseIfStatement() {
  auto statement = std::make_unique<IfStatement>();
  statement->line = current().line;
  statement->column = current().column;

  expect(TokenType::IF);
  expect(TokenType::LPAREN);

  statement->condition = parseExpression();

  expect(TokenType::RPAREN);
  expect(TokenType::LBRACE);

  while (current().type != TokenType::RBRACE) {
    statement->thenBranch.push_back(parseStatement());
  }

  expect(TokenType::RBRACE);

  if (current().type == TokenType::ELSE) {
    advance();
    if (current().type == TokenType::IF) {
      statement->elseBranch.push_back(parseIfStatement());
    } else {

      expect(TokenType::LBRACE);

      while (current().type != TokenType::RBRACE) {
        statement->elseBranch.push_back(parseStatement());
      }

      expect(TokenType::RBRACE);
    }
  }

  return statement;
}

std::unique_ptr<Statement> Parser::parseWhileStatement() {
  auto statement = std::make_unique<WhileStatement>();
  statement->line = current().line;
  statement->column = current().column;
  expect(TokenType::WHILE);
  expect(TokenType::LPAREN);

  statement->condition = parseExpression();

  expect(TokenType::RPAREN);
  expect(TokenType::LBRACE);

  while (current().type != TokenType::RBRACE) {
    statement->body.push_back(parseStatement());
  }

  expect(TokenType::RBRACE);

  return statement;
}

std::unique_ptr<Expression> Parser::parseArray() {
  expect(TokenType::LBRACKET);

  auto array = std::make_unique<ArrayExpression>();

  if (current().type == TokenType::RBRACKET) {
    advance();
    return array;
  }

  array->elements.push_back(parseExpression());

  while (current().type == TokenType::COMMA) {
    advance();
    array->elements.push_back(parseExpression());
  }

  expect(TokenType::RBRACKET);

  return array;
}

std::unique_ptr<Expression> Parser::parsePostfix() {
  auto expression = parsePrimary();

  while (true) {
    // Function call: foo(...)
    if (current().type == TokenType::LPAREN) {
      advance(); // consume '('

      auto call = std::make_unique<CallExpression>();
      call->callee = std::move(expression);

      if (current().type != TokenType::RPAREN) {
        while (true) {
          call->arguments.push_back(parseExpression());

          if (current().type != TokenType::COMMA)
            break;

          advance(); // consume ','
        }
      }

      expect(TokenType::RPAREN);

      expression = std::move(call);
      continue;
    }

    // Array indexing: foo[index]
    if (current().type == TokenType::LBRACKET) {
      advance(); // consume '['

      auto index = parseExpression();

      expect(TokenType::RBRACKET);

      auto indexed = std::make_unique<IndexExpression>();

      indexed->object = std::move(expression);
      indexed->index = std::move(index);

      expression = std::move(indexed);
      continue;
    }

    break;
  }

  return expression;
}
std::unique_ptr<Statement> Parser::parseFunction() {
  int line = current().line;
  int column = current().column;
  advance(); // consume 'fn'

  // Function name
  std::string name = current().value;
  expect(TokenType::IDENTIFIER);

  // Parameters
  expect(TokenType::LPAREN);

  std::vector<FunctionStatement::Parameter> parameters;

  if (current().type != TokenType::RPAREN) {
    while (true) {
      // Parameter name
      std::string parameterName = current().value;
      expect(TokenType::IDENTIFIER);

      // :
      expect(TokenType::COLON);

      // Parameter type
      Type parameterType = parseType();

      FunctionStatement::Parameter parameter{parameterName,
                                             std::move(parameterType), nullptr};

      // Optional default value
      if (current().type == TokenType::EQUAL) {
        advance(); // consume '='
        parameter.defaultValue = parseExpression();
      }

      parameters.push_back(std::move(parameter));

      if (current().type != TokenType::COMMA)
        break;

      advance(); // consume ','
    }
  }

  expect(TokenType::RPAREN);

  // :
  expect(TokenType::COLON);

  // Return type
  Type returnType = parseType();

  // Construct FunctionStatement
  auto function = std::make_unique<FunctionStatement>(
      std::move(name), std::move(parameters), std::move(returnType));

  function->line = line;
  function->column = column;

  // Function body
  expect(TokenType::LBRACE);

  while (current().type != TokenType::RBRACE) {
    if (current().type == TokenType::END_OF_FILE) {
      throw std::runtime_error("PARSER ERROR: Expected '}' before end of file");
    }

    function->body.push_back(parseStatement());
  }

  expect(TokenType::RBRACE);

  return function;
}

std::unique_ptr<Statement> Parser::parseReturn() {
  expect(TokenType::RETURN);

  auto statement = std::make_unique<ReturnStatement>();
  statement->line = current().line;
  statement->column = current().column;
  statement->value = parseExpression();

  expect(TokenType::SEMICOLON);

  return statement;
}

Program Parser::parseProgram() {
  Program program;

  while (current().type != TokenType::END_OF_FILE) {
    program.statements.push_back(parseStatement());
  }

  return program;
}

std::unique_ptr<Statement> Parser::parsePrint() {
  advance(); // print
  expect(TokenType::LPAREN);
  auto statement = std::make_unique<PrintStatement>();
  statement->line = current().line;
  statement->column = current().column;
  statement->values.push_back(parseExpression());
  while (current().type == TokenType::COMMA) {
    advance();
    statement->values.push_back(parseExpression());
  }
  expect(TokenType::RPAREN);
  expect(TokenType::SEMICOLON);

  return statement;
}
