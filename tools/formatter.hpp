#pragma once

#include "../src/ast.hpp"
#include <string>

class Formatter {
public:
  std::string format(const Program &program, const std::vector<Token> &tokens);

private:
  std::string output;
  int indentLevel = 0;

  void formatStatement(const Statement *statement);
  void formatExpression(const Expression *expression, int parentPrecedence = 0);

  void writeIndent();

  int precedence(TokenType type);

  std::vector<Token> comments;
  std::size_t commentPosition = 0;

  void formatCommentsBefore(int line);
  void formatTrailingComment(int line);
  void formatBlock(const std::vector<std::unique_ptr<Statement>> &statements,
                   int blockStartLine);
};