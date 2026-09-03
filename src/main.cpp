#include <iostream>

#include "interpreter.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int main() {
  std::string source = "let x = 10;"
                       "let y = x + 20;"
                       "print(x);"
                       "print(y);"
                       "print(x * y);";

  Lexer lexer(source);
  auto tokens = lexer.tokenize();

  Parser parser(tokens);
  Program program = parser.parseProgram();

  Interpreter interpreter;
  interpreter.execute(program);

  return 0;
}