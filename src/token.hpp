#pragma once

#include <string>

enum class TokenType
{
    LET,
    INTEGER,
    IDENTIFIER,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    EQUAL,
    SEMICOLON,
    INVALID,
    END_OF_FILE
};

struct Token
{
    TokenType type;
    std::string value;
    int line;
    int column;
};