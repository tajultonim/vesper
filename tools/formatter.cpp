#include <sstream>
#include <stdexcept>

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

  case TokenType::PERCENT:
    return "%";

  case TokenType::SLASH_SLASH:
    return "//";

  case TokenType::STAR_STAR:
    return "**";

  case TokenType::EQUAL_EQUAL:
    return "==";

  case TokenType::NOT_EQUAL:
    return "!=";

  case TokenType::LESS:
    return "<";

  case TokenType::GREATER:
    return ">";

    // ...

  default:
    throw std::runtime_error("Unknown operator");
  }
}

std::string typeToString(const Type &type) {
  switch (type.kind) {
  case Type::Kind::INT:
    return "int";

  case Type::Kind::BOOL:
    return "bool";

  case Type::Kind::FLOAT:
    return "float";

  case Type::Kind::STRING:
    return "string";

  case Type::Kind::ARRAY:
    return "[" + typeToString(*type.elementType) + "]";
  }

  throw std::runtime_error("Unknown type");
}

int Formatter::precedence(TokenType type) {
  switch (type) {
  case TokenType::EQUAL_EQUAL:
  case TokenType::NOT_EQUAL:
  case TokenType::LESS:
  case TokenType::GREATER:
    return 1;

  case TokenType::PLUS:
  case TokenType::MINUS:
    return 2;

  case TokenType::STAR:
  case TokenType::SLASH:
  case TokenType::PERCENT:
  case TokenType::SLASH_SLASH:
    return 3;

  case TokenType::STAR_STAR:
    return 4;

  default:
    return 0;
  }
}

void Formatter::formatExpression(const Expression *expression,
                                 int parentPrecedence) {
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
  } else if (auto *array = dynamic_cast<const ArrayExpression *>(expression)) {
    output += "[";

    for (std::size_t i = 0; i < array->elements.size(); ++i) {
      if (i > 0)
        output += ", ";

      formatExpression(array->elements[i].get());
    }

    output += "]";
  }
  else if (auto *index = dynamic_cast<const IndexExpression *>(expression)) {
    formatExpression(index->object.get());
    output += "[";
    formatExpression(index->index.get());
    output += "]";
  } else if (auto *unary = dynamic_cast<const UnaryExpression *>(expression)) {
    output += operatorToString(unary->operatorType);

    formatExpression(unary->operand.get(), 4);
  } else if (auto *binary =
                 dynamic_cast<const BinaryExpression *>(expression)) {
    int currentPrecedence = precedence(binary->operatorType);

    bool needsParentheses = currentPrecedence < parentPrecedence;

    if (needsParentheses)
      output += "(";

    formatExpression(binary->left.get(), currentPrecedence);

    output += " ";
    output += operatorToString(binary->operatorType);
    output += " ";

    formatExpression(binary->right.get(), currentPrecedence);

    if (needsParentheses)
      output += ")";
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

    output += assignment->name;
    output += " = ";

    formatExpression(assignment->value.get());

    output += ";\n";
  } else if (auto *print = dynamic_cast<const PrintStatement *>(statement)) {
    writeIndent();

    output += "print(";

    formatExpression(print->value.get());

    output += ");\n";
  } else if (auto *ifStatement = dynamic_cast<const IfStatement *>(statement)) {
    writeIndent();

    output += "if (";
    formatExpression(ifStatement->condition.get());
    output += ") {\n";

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
  } else if (auto *whileStatement =
                 dynamic_cast<const WhileStatement *>(statement)) {
    writeIndent();

    output += "while (";
    formatExpression(whileStatement->condition.get());
    output += ") {\n";

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
