#pragma once

#include <string>
#include <memory>

struct Expression
{
    virtual ~Expression() = default;
};

struct LetStatement
{
    std::string name;
    int value;
};

struct BinaryExpression : Expression
{
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    TokenType operatorType;
};

struct IntegerExpression : Expression
{
    int value;
};