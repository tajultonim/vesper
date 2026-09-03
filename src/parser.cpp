#include "parser.hpp"
#include <iostream>

Parser::Parser(const std::vector<Token> &input)
    : tokens(input)
{
}

LetStatement Parser::parseLet()
{
    LetStatement statement;

    expect(TokenType::LET);

    statement.name = current().value;
    expect(TokenType::IDENTIFIER);

    expect(TokenType::EQUAL);

    statement.value = std::stoi(current().value);
    expect(TokenType::INTEGER);

    expect(TokenType::SEMICOLON);

    return statement;
}

Token Parser::current() const
{
    return tokens[position];
}

std::string tokenTypeName(TokenType type)
{
    switch (type)
    {
        case TokenType::LET:
            return "let";

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

        case TokenType::EQUAL:
            return "=";

        case TokenType::SEMICOLON:
            return ";";

        case TokenType::INVALID:
            return "invalid";

        case TokenType::END_OF_FILE:
            return "end of file";
    }

    return "unknown";
}

void Parser::advance()
{
    if (position < tokens.size())
    {
        position++;
    }
}

bool Parser::expect(TokenType type)
{
    if (current().type != type)
    {
        std::cerr << "Parser error at line "
                  << current().line
                  << ", column "
                  << current().column
                  << ": expected "
                  << tokenTypeName(type)
                  << ", got "
                  << tokenTypeName(current().type)
                  << '\n';

        return false;
    }

    advance();
    return true;
}


BinaryExpression Parser::parseExpression()
{
    BinaryExpression expression;

    auto left = std::make_unique<IntegerExpression>();
    left->value = std::stoi(current().value);
    expression.left = std::move(left);
    expect(TokenType::INTEGER);

    expression.operatorType = current().type;
    expect(TokenType::PLUS);

    auto right = std::make_unique<IntegerExpression>();
    right->value = std::stoi(current().value);
    expression.right = std::move(right);
    expect(TokenType::INTEGER);

    return expression;
}

std::unique_ptr<Expression> Parser::parsePrimary()
{
    if (current().type == TokenType::INTEGER)
    {
        auto expression = std::make_unique<IntegerExpression>();

        expression->value = std::stoi(current().value);

        advance();

        return expression;
    }

    return nullptr;
}