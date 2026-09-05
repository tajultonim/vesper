#pragma once

#include "ast.hpp"
#include <string>

class Formatter
{
public:
    std::string format(const Program& program);

private:
    std::string output;
    int indentLevel = 0;

    void formatStatement(const Statement* statement);
    void formatExpression(const Expression* expression);

    void writeIndent();
};