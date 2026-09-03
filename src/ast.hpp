#pragma once

#include <string>
#include <memory>

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

struct Expression
{
    virtual ~Expression() = default;
};

struct IntegerExpression : Expression
{
    int value;
};