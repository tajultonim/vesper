#include "type_checker.hpp"

#include <stdexcept>
#include <string>

bool sameType(const Type &a, const Type &b) {
  if (a.kind != b.kind)
    return false;

  if (a.kind == Type::Kind::ARRAY) {
    return sameType(*a.elementType, *b.elementType);
  }

  return true;
}

bool isNumeric(Type::Kind type) {
  return type == Type::Kind::INT || type == Type::Kind::FLOAT;
}

void requireNumericOperands(Type::Kind left, Type::Kind right,
                            const std::string &operatorSymbol) {
  if (!isNumeric(left) || !isNumeric(right)) {
    throw std::runtime_error("TYPE ERROR: operator '" + operatorSymbol +
                             "' requires numeric operands");
  }
}

Type TypeChecker::checkExpression(const Expression *expression) {
  if (dynamic_cast<const IntegerExpression *>(expression)) {
    return Type(Type::Kind::INT);
  }

  if (dynamic_cast<const StringExpression *>(expression)) {
    return Type(Type::Kind::STRING);
  }

  if (dynamic_cast<const FloatExpression *>(expression)) {
    return Type(Type::Kind::FLOAT);
  }

  if (dynamic_cast<const BooleanExpression *>(expression)) {
    return Type(Type::Kind::BOOL);
  }

  if (auto *identifier =
          dynamic_cast<const IdentifierExpression *>(expression)) {
    auto it = types.find(identifier->name);

    if (it == types.end()) {
      throw std::runtime_error("TYPE ERROR: Undefined variable '" +
                               identifier->name + "'");
    }

    return it->second.type;
  }

  if (auto *array = dynamic_cast<const ArrayExpression *>(expression)) {
    if (array->elements.empty()) {
      throw std::runtime_error("TYPE ERROR: cannot infer type of empty array");
    }

    Type elementType = checkExpression(array->elements[0].get());

    for (std::size_t i = 1; i < array->elements.size(); ++i) {
      Type currentType = checkExpression(array->elements[i].get());

      if (!sameType(elementType, currentType)) {
        throw std::runtime_error(
            "TYPE ERROR: array elements must have the same type");
      }
    }

    return Type(Type::Kind::ARRAY,
                std::make_unique<Type>(std::move(elementType)));
  }

  if (auto *index = dynamic_cast<const IndexExpression *>(expression)) {
    Type objectType = checkExpression(index->object.get());
    Type indexType = checkExpression(index->index.get());

    if (objectType.kind != Type::Kind::ARRAY) {
      throw std::runtime_error("TYPE ERROR: cannot index a non-array value");
    }

    if (indexType.kind != Type::Kind::INT) {
      throw std::runtime_error("TYPE ERROR: array index must be an integer");
    }

    return *objectType.elementType;
  }

  if (auto *unary = dynamic_cast<const UnaryExpression *>(expression)) {
    Type operandType = checkExpression(unary->operand.get());

    switch (unary->operatorType) {
    case TokenType::MINUS:
      if (!isNumeric(operandType.kind)) {
        throw std::runtime_error(
            "TYPE ERROR: unary '-' operator requires a numeric operand");
      }
      return operandType;

    case TokenType::PLUS:
      if (!isNumeric(operandType.kind)) {
        throw std::runtime_error(
            "TYPE ERROR: unary '+' operator requires a numeric operand");
      }
      return operandType;

    default:
      throw std::runtime_error("TYPE ERROR: Unknown unary operator");
    }
  }

  if (auto *binary = dynamic_cast<const BinaryExpression *>(expression)) {
    Type left = checkExpression(binary->left.get());
    Type right = checkExpression(binary->right.get());

    switch (binary->operatorType) {
    case TokenType::PLUS:

      if (left.kind == Type::Kind::STRING && right.kind == Type::Kind::STRING) {
        return Type(Type::Kind::STRING);
      }

      requireNumericOperands(left.kind, right.kind, "+");

      if (left.kind == Type::Kind::FLOAT || right.kind == Type::Kind::FLOAT) {
        return Type(Type::Kind::FLOAT);
      }

      return Type(Type::Kind::INT);

    case TokenType::MINUS:

      requireNumericOperands(left.kind, right.kind, "-");

      if (left.kind == Type::Kind::FLOAT || right.kind == Type::Kind::FLOAT) {
        return Type(Type::Kind::FLOAT);
      }

      return Type(Type::Kind::INT);

    case TokenType::STAR:

      if (left.kind == Type::Kind::STRING && right.kind == Type::Kind::INT) {
        return Type(Type::Kind::STRING);
      }

      requireNumericOperands(left.kind, right.kind, "*");

      if (left.kind == Type::Kind::FLOAT || right.kind == Type::Kind::FLOAT) {
        return Type(Type::Kind::FLOAT);
      }

      return Type(Type::Kind::INT);

    case TokenType::SLASH:

      requireNumericOperands(left.kind, right.kind, "/");

      if (left.kind == Type::Kind::FLOAT || right.kind == Type::Kind::FLOAT) {
        return Type(Type::Kind::FLOAT);
      }

      return Type(Type::Kind::INT);

    case TokenType::LESS:

      requireNumericOperands(left.kind, right.kind, "<");

      return Type(Type::Kind::BOOL);

    case TokenType::LESS_EQUAL:

      requireNumericOperands(left.kind, right.kind, "<=");

      return Type(Type::Kind::BOOL);

    case TokenType::GREATER:

      requireNumericOperands(left.kind, right.kind, ">");

      return Type(Type::Kind::BOOL);

    case TokenType::GREATER_EQUAL:

      requireNumericOperands(left.kind, right.kind, ">=");

      return Type(Type::Kind::BOOL);

    case TokenType::EQUAL_EQUAL:

      if (!sameType(left, right)) {
        throw std::runtime_error("TYPE ERROR: operands of '==' "
                                 "must have the same type");
      }

      return Type(Type::Kind::BOOL);

    case TokenType::STAR_STAR:
      requireNumericOperands(left.kind, right.kind, "**");
      if (left.kind == Type::Kind::FLOAT || right.kind == Type::Kind::FLOAT) {
        return Type(Type::Kind::FLOAT);
      }
      return Type(Type::Kind::INT);

    case TokenType::SLASH_SLASH:
      requireNumericOperands(left.kind, right.kind, "//");
      return Type(Type::Kind::INT);

    case TokenType::PERCENT:
      requireNumericOperands(left.kind, right.kind, "%");
      return Type(Type::Kind::INT);

    case TokenType::NOT_EQUAL:

      if (!sameType(left, right)) {
        throw std::runtime_error("TYPE ERROR: operands of '!=' "
                                 "must have the same type");
      }

      return Type(Type::Kind::BOOL);

    default:

      throw std::runtime_error("TYPE ERROR: Unknown binary operator");
    }
  }

  throw std::runtime_error("TYPE ERROR: Unknown expression");
}

void TypeChecker::checkAssignment(const AssignmentStatement *assignment) {
  auto it = types.find(assignment->name);

  if (it == types.end()) {
    throw std::runtime_error("TYPE ERROR: Undefined variable '" +
                             assignment->name + "'");
  }

  Type valueType = checkExpression(assignment->value.get());

  if (!it->second.mutable_) {
    throw std::runtime_error(
        "TYPE ERROR: Cannot assign to immutable variable '" + assignment->name +
        "'");
  }

  if (!sameType(it->second.type, valueType)) {
    throw std::runtime_error(
        "TYPE ERROR: cannot assign value of different type");
  }
}

void TypeChecker::checkDeclaration(const VariableDeclaration *declaration) {
  Type expressionType = checkExpression(declaration->value.get());

  if (declaration->declaredType.has_value()) {
    if (!sameType(expressionType, declaration->declaredType.value())) {
      throw std::runtime_error("TYPE ERROR: declared type does not "
                               "match initializer");
    }
  }

  types.insert_or_assign(declaration->name,
                         VariableInfo{expressionType, declaration->mutable_});
}

void TypeChecker::checkStatement(const Statement *statement) {
  if (auto *declaration =
          dynamic_cast<const VariableDeclaration *>(statement)) {
    checkDeclaration(declaration);
  } else if (auto *assignment =
                 dynamic_cast<const AssignmentStatement *>(statement)) {
    checkAssignment(assignment);
  } else if (auto *print = dynamic_cast<const PrintStatement *>(statement)) {
    for (const auto &printValue : print->values) {
      checkExpression(printValue.get());
    }
  } else if (auto *ifStatement = dynamic_cast<const IfStatement *>(statement)) {
    checkIfStatement(ifStatement);
  } else if (auto *whileStatement =
                 dynamic_cast<const WhileStatement *>(statement)) {
    checkWhileStatement(whileStatement);
  } else {
    throw std::runtime_error("TYPE ERROR: Unknown statement");
  }
}

void TypeChecker::checkIfStatement(const IfStatement *ifStatement) {
  Type conditionType = checkExpression(ifStatement->condition.get());

  if (conditionType.kind != Type::Kind::BOOL) {
    throw std::runtime_error("TYPE ERROR: condition of 'if' "
                             "statement must be a boolean");
  }

  for (const auto &statement : ifStatement->thenBranch) {
    checkStatement(statement.get());
  }

  for (const auto &statement : ifStatement->elseBranch) {
    checkStatement(statement.get());
  }
}

void TypeChecker::checkWhileStatement(const WhileStatement *whileStatement) {
  Type conditionType = checkExpression(whileStatement->condition.get());

  if (conditionType.kind != Type::Kind::BOOL) {
    throw std::runtime_error("TYPE ERROR: condition of 'while' "
                             "statement must be a boolean");
  }

  for (const auto &statement : whileStatement->body) {
    checkStatement(statement.get());
  }
}

void TypeChecker::checkProgram(const Program &program) {
  for (const auto &statement : program.statements) {
    checkStatement(statement.get());
  }
}