#include "lexer.hpp"

Lexer::Lexer(const std::string &input)
    : source(input)
{
}

char Lexer::current() const
{
    if (position >= source.size())
    {
        return '\0';
    }

    return source[position];
}

void Lexer::advance()
{
    if (position < source.size())
    {
        position++;
    }
}

Token Lexer::readNumber()
{
    std::string c;

    while (current() >= '0' && current() <= '9')
    {
        c += current();
        advance();
    }

    return Token{
        TokenType::INTEGER,
        c,
        1,
        static_cast<int>(position) + 1};
}

void Lexer::skipWhitespace()
{
    while (current() == ' ' ||
           current() == '\n' ||
           current() == '\t')
    {
        advance();
    }
}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;

    while (position < source.size())
    {
        skipWhitespace();

        if (position >= source.size())
        {
            break;
        }

        char c = current();

        if (c >= '0' && c <= '9')
        {
            tokens.push_back(readNumber());
        }
        else if ((c >= 'a' && c <= 'z') ||
                 (c >= 'A' && c <= 'Z') ||
                 c == '_')
        {
            tokens.push_back(readIdentifier());
        }
        else if (c == '=')
        {

            tokens.push_back(Token{
                TokenType::EQUAL,
                "=",
                1,
                static_cast<int>(position) + 1});

            advance();
        }
        else if (c == ';')
        {
            tokens.push_back(Token{
                TokenType::SEMICOLON,
                ";",
                1,
                static_cast<int>(position) + 1});

            advance();
        }

        else if (c == '+')
        {
            tokens.push_back(Token{
                TokenType::PLUS,
                "+",
                1,
                static_cast<int>(position) + 1});
            advance();
        }
        else if (c == '-')
        {
            tokens.push_back(Token{
                TokenType::MINUS,
                "-",
                1,
                static_cast<int>(position) + 1});
            advance();
        }
        else if (c == '*')
        {
            tokens.push_back(Token{
                TokenType::STAR,
                "*",
                1,
                static_cast<int>(position) + 1});
            advance();
        }
        else if (c == '/')
        {
            tokens.push_back(Token{
                TokenType::SLASH,
                "/",
                1,
                static_cast<int>(position) + 1});
            advance();
        }
        else
        {
            tokens.push_back(Token{
                TokenType::INVALID,
                std::string(1, c),
                1,
                static_cast<int>(position) + 1});

            advance();
        }
    }
    tokens.push_back(Token{
        TokenType::END_OF_FILE,
        "",
        1,
        static_cast<int>(position) + 1});

    return tokens;
}

Token Lexer::readIdentifier()
{
    std::string c;

    while ((current() >= 'a' && current() <= 'z') ||
           (current() >= 'A' && current() <= 'Z') ||
           (current() >= '0' && current() <= '9') ||
           current() == '_')
    {
        c += current();
        advance();
    }

    if (c == "let")
    {
        return Token{
            TokenType::LET,
            c,
            1,
            static_cast<int>(position) + 1};
    }

    return Token{
        TokenType::IDENTIFIER,
        c,
        1,
        static_cast<int>(position) + 1};
}