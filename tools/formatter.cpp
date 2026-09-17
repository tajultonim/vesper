#include <iostream>
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

  case TokenType::LESS_EQUAL:
    return "<=";

  case TokenType::GREATER_EQUAL:
    return ">=";

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

  case Type::Kind::VOID:
    return "void";
  }

  throw std::runtime_error("Unknown type");
}

std::string escapeString(const std::string &value) {
  std::string result;

  for (char c : value) {
    switch (c) {
    case '\n':
      result += "\\n";
      break;

    case '\t':
      result += "\\t";
      break;

    case '\\':
      result += "\\\\";
      break;

    case '"':
      result += "\\\"";
      break;

    default:
      result += c;
      break;
    }
  }

  return result;
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

/*
 * Format all statements inside a block.
 *
 * blockStartLine is important because comments before the
 * block belong to the statement that opened the block.
 *
 * Example:
 *
 * # function comment
 * fn test(): int {
 *     # body comment
 *     return 1;
 * }
 *
 * "# function comment" belongs to fn.
 * "# body comment" belongs to the function body.
 */
void Formatter::formatBlock(
    const std::vector<std::unique_ptr<Statement>> &statements,
    int blockStartLine) {
  for (const auto &stmt : statements) {
    /*
     * Only consume comments that occur AFTER the line
     * where this block started.
     *
     * Comments before the block were handled by the
     * parent formatter.
     */
    if (stmt->line > blockStartLine) {
      formatCommentsBefore(stmt->line);
    }

    formatStatement(stmt.get());

    formatTrailingComment(stmt->line);
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

    std::string value = stream.str();

    if (value.find('.') == std::string::npos &&
        value.find('e') == std::string::npos &&
        value.find('E') == std::string::npos) {
      value += ".0";
    }

    output += value;
  } else if (auto *boolean =
                 dynamic_cast<const BooleanExpression *>(expression)) {
    output += boolean->value ? "true" : "false";
  } else if (auto *string =
                 dynamic_cast<const StringExpression *>(expression)) {
    output += "\"" + escapeString(string->value) + "\"";
  } else if (auto *identifier =
                 dynamic_cast<const IdentifierExpression *>(expression)) {
    output += identifier->name;
  } else if (auto *member = dynamic_cast<const MemberExpression *>(expression)) {
    formatExpression(member->object.get());
    output += ".";
    output += member->member;
  } else if (auto *array = dynamic_cast<const ArrayExpression *>(expression)) {
    output += "[";

    for (std::size_t i = 0; i < array->elements.size(); ++i) {
      if (i > 0)
        output += ", ";

      formatExpression(array->elements[i].get());
    }

    output += "]";
  } else if (auto *index = dynamic_cast<const IndexExpression *>(expression)) {
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
  } else if (auto *call = dynamic_cast<const CallExpression *>(expression)) {
    formatExpression(call->callee.get());

    output += "(";

    for (std::size_t i = 0; i < call->arguments.size(); ++i) {
      if (i > 0)
        output += ", ";

      formatExpression(call->arguments[i].get());
    }

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
  }

  else if (auto *assignment =
               dynamic_cast<const AssignmentStatement *>(statement)) {
    writeIndent();

    output += assignment->name;
    output += " = ";

    formatExpression(assignment->value.get());

    output += ";\n";
  }

  else if (auto *expressionStatement =
               dynamic_cast<const ExpressionStatement *>(statement)) {
    writeIndent();
    formatExpression(expressionStatement->expression.get());
    output += ";\n";
  }

  else if (auto *importStatement =
               dynamic_cast<const ImportStatement *>(statement)) {
    writeIndent();
    output += "import \"";
    output += escapeString(importStatement->path);
    output += "\" as ";
    output += importStatement->alias;
    output += ";\n";
  }

 
  else if (auto *ifStatement = dynamic_cast<const IfStatement *>(statement)) {
    writeIndent();

    output += "if (";

    formatExpression(ifStatement->condition.get());

    output += ") {\n";

    indentLevel++;

    formatBlock(ifStatement->thenBranch, ifStatement->line);

    indentLevel--;

    writeIndent();

    output += "}";

    if (!ifStatement->elseBranch.empty()) {
      output += " else {\n";

      indentLevel++;

      formatBlock(ifStatement->elseBranch, ifStatement->line);

      indentLevel--;

      writeIndent();

      output += "}";
    }

    output += "\n";
  }

  else if (auto *function =
               dynamic_cast<const FunctionStatement *>(statement)) {
    writeIndent();

    if (function->isExport) {
      output += "export ";
    }

    if (function->isExtern) {
      output += "extern ";
    }

    output += "fn ";
    output += function->name;
    output += "(";

    for (std::size_t i = 0; i < function->parameters.size(); ++i) {
      if (i > 0)
        output += ", ";

      const auto &param = function->parameters[i];

      output += param.name;
      output += ": ";
      output += typeToString(param.type);

      if (param.defaultValue) {
        output += " = ";

        formatExpression(param.defaultValue.get());
      }
    }

    output += "): ";
    output += typeToString(function->returnType);

    if (function->isExtern) {
      output += ";\n";
      return;
    }

    output += " {\n";

    indentLevel++;

    formatBlock(function->body, function->line);

    indentLevel--;

    writeIndent();

    output += "}\n";
  }

  else if (auto *returnStatement =
               dynamic_cast<const ReturnStatement *>(statement)) {
    writeIndent();

    output += "return";

    if (returnStatement->value) {
      output += " ";
      formatExpression(returnStatement->value.get());
    }

    output += ";\n";
  }

  else if (auto *whileStatement =
               dynamic_cast<const WhileStatement *>(statement)) {
    writeIndent();

    output += "while (";

    formatExpression(whileStatement->condition.get());

    output += ") {\n";

    indentLevel++;

    formatBlock(whileStatement->body, whileStatement->line);

    indentLevel--;

    writeIndent();

    output += "}\n";
  }
}

/*
 * Format comments that appear before a statement.
 */
void Formatter::formatCommentsBefore(int line) {
  while (commentPosition < comments.size()) {
    const Token &comment = comments[commentPosition];

    /*
     * A comment on the same line belongs to the
     * statement on that line and is handled by
     * formatTrailingComment().
     */
    if (comment.line >= line)
      break;

    writeIndent();

    output += comment.value;
    output += "\n";

    ++commentPosition;
  }
}

/*
 * Format comments appearing after a statement on
 * the same source line.
 */
void Formatter::formatTrailingComment(int line) {
  while (commentPosition < comments.size()) {
    const Token &comment = comments[commentPosition];

    if (comment.line != line)
      break;

    /*
     * formatStatement() already produced '\n'.
     */
    if (!output.empty() && output.back() == '\n') {
      output.pop_back();
    }

    output += " ";
    output += comment.value;
    output += "\n";

    ++commentPosition;
  }
}

std::string Formatter::format(const Program &program,
                              const std::vector<Token> &tokens) {
  output.clear();
  indentLevel = 0;

  comments.clear();
  commentPosition = 0;

  /*
   * Keep comments from the original token stream.
   *
   * The parser may ignore COMMENT tokens, but the
   * formatter still has access to them here.
   */
  for (const auto &token : tokens) {
    if (token.type == TokenType::COMMENT) {
      comments.push_back(token);
    }
  }

  enum class StatementKind { Other, VarDecl, FuncDecl, Call };

  StatementKind prevKind = StatementKind::Other;

  /*
   * Top-level statements.
   */
  for (const auto &statement : program.statements) {
    const Statement *stmtPtr = statement.get();

    /*
     * IMPORTANT:
     *
     * This consumes comments BEFORE the top-level
     * statement, including comments before a function.
     */
    formatCommentsBefore(stmtPtr->line);

    StatementKind currentKind = StatementKind::Other;

    if (dynamic_cast<const VariableDeclaration *>(stmtPtr)) {
      currentKind = StatementKind::VarDecl;
    } else if (dynamic_cast<const FunctionStatement *>(stmtPtr)) {
      currentKind = StatementKind::FuncDecl;
    } else if (auto *assign =
                   dynamic_cast<const AssignmentStatement *>(stmtPtr)) {
      if (dynamic_cast<const CallExpression *>(assign->value.get())) {
        currentKind = StatementKind::Call;
      }
    }

    if (prevKind != StatementKind::Other && prevKind != currentKind) {
      output += "\n";
    }

    formatStatement(stmtPtr);

    formatTrailingComment(stmtPtr->line);

    prevKind = currentKind;
  }

  /*
   * Any comments remaining after the final statement.
   */
  while (commentPosition < comments.size()) {
    const Token &comment = comments[commentPosition];

    writeIndent();

    output += comment.value;
    output += "\n";

    ++commentPosition;
  }

  return output;
}

void Formatter::writeIndent() { output += std::string(indentLevel * 4, ' '); }