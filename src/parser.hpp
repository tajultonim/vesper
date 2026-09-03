#pragma once
#include <memory>

#include <vector>
#include "token.hpp"
#include "ast.hpp"

class Parser
{
public:
    Parser(const std::vector<Token> &tokens);

    Token current() const;
    void advance();

    bool expect(TokenType type);

    std::unique_ptr<Expression> parsePrimary();

private:
    std::vector<Token> tokens;
    std::size_t position = 0;
};