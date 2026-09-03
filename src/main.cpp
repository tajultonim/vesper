#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"

int main()
{
    std::string source = "10";

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);

    BinaryExpression expression = parser.parseExpression();

    std::cout << "left: " << expression.left << '\n';
    std::cout << "right: " << expression.right << '\n';
    expression.operatorType == TokenType::PLUS ? std::cout << "operator: +" << '\n' : std::cout << "operator: -" << '\n';

    return 0;
}