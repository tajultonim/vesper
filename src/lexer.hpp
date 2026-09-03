#pragma once

#include <string>
#include <vector>

#include "token.hpp"

class Lexer
{
public:
    Lexer(const std::string &source);

    char current() const;
    void advance();
    std::vector<Token> tokenize();

private:
    std::string source;
    std::size_t position = 0;
    Token readNumber();
    Token readIdentifier();
    void skipWhitespace();
};