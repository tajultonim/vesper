#include <sstream>

#include "formatter.hpp"

std::string operatorToString(TokenType type) {
  switch (type) {
  case TokenType::PLUS:
    return "+";
  case TokenType::MINUS:
    return "-";
  case TokenType::STAR:
    return "*";
  case TokenType::SLASH:
    return "/";
  case TokenType::EQUAL_EQUAL:
    return "==";
  case TokenType::NOT_EQUAL:
    return "!=";
  case TokenType::LESS:
    return "<";
  case TokenType::GREATER:
    return ">";
    // ...
  }

  throw std::runtime_error("Unknown operator");
}

std::string typeToString(Type type) {
  switch (type) {
  case Type::INT:
    return "int";
  case Type::BOOL:
    return "bool";
  case Type::FLOAT:
    return "float";
  case Type::STRING:
    return "string";
  }

  throw std::runtime_error("Unknown type");
}

void Formatter::formatExpression(const Expression *expression) {
  if (auto *integer = dynamic_cast<const IntegerExpression *>(expression)) {
    output += std::to_string(integer->value);
  } else if (auto *floating =
                 dynamic_cast<const FloatExpression *>(expression)) {

    std::ostringstream stream;
    stream << floating->value;
    output += stream.str();
  } else if (auto *boolean =
                 dynamic_cast<const BooleanExpression *>(expression)) {
    output += boolean->value ? "true" : "false";
  } else if (auto *string =
                 dynamic_cast<const StringExpression *>(expression)) {
    output += "\"" + string->value + "\"";
  } else if (auto *identifier =
                 dynamic_cast<const IdentifierExpression *>(expression)) {
    output += identifier->name;
  } else if (auto *binary =
                 dynamic_cast<const BinaryExpression *>(expression)) {
    formatExpression(binary->left.get());

    output += " " + operatorToString(binary->operatorType) + " ";

    formatExpression(binary->right.get());
  } else {
    throw std::runtime_error("Unknown expression");
  }
}

void Formatter::formatStatement(const Statement *statement) {
  if (auto *declaration =
          dynamic_cast<const VariableDeclaration *>(statement)) {
    writeIndent();

    output += declaration->mutable_ ? "mut " : "let ";

    output += declaration->name;

    if (declaration->declaredType) {
      output += ": ";
      output += typeToString(*declaration->declaredType);
    }

    output += " = ";

    formatExpression(declaration->value.get());

    output += ";\n";
  } else if (auto *assignment =
                 dynamic_cast<const AssignmentStatement *>(statement)) {
    writeIndent();

    output += assignment->name + " = ";

    formatExpression(assignment->value.get());

    output += ";\n";
  }

  else if (auto *print = dynamic_cast<const PrintStatement *>(statement)) {
    writeIndent();

    output += "print(";

    formatExpression(print->value.get());

    output += ");\n";
  } else if (auto *ifStatement = dynamic_cast<const IfStatement *>(statement)) {
    writeIndent();
    output += "if (";
    formatExpression(ifStatement->condition.get());
    output += " ){\n";

    indentLevel++;

    for (const auto &stmt : ifStatement->thenBranch) {
      formatStatement(stmt.get());
    }

    indentLevel--;

    writeIndent();
    output += "}";

    if (!ifStatement->elseBranch.empty()) {
      output += " else {\n";

      indentLevel++;

      for (const auto &stmt : ifStatement->elseBranch) {
        formatStatement(stmt.get());
      }

      indentLevel--;

      writeIndent();
      output += "}";
    }

    output += "\n";
  }

  else if (auto *whileStatement =
               dynamic_cast<const WhileStatement *>(statement)) {

    output += "\n";
    writeIndent();
    output += "while (";
    formatExpression(whileStatement->condition.get());
    output += " ){\n";

    indentLevel++;

    for (const auto &stmt : whileStatement->body) {
      formatStatement(stmt.get());
    }

    indentLevel--;

    writeIndent();
    output += "}\n";
  }
}

std::string Formatter::format(const Program &program) {
  output.clear();
  indentLevel = 0;

  for (const auto &statement : program.statements) {
    formatStatement(statement.get());
  }

  return output;
}

void Formatter::writeIndent() { output += std::string(indentLevel * 4, ' '); }