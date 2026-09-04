#include <iostream>
#include <stdexcept>
#include <type_traits>

#include "interpreter.hpp"

//*****************************************
// Helper functions
//*****************************************

std::string valueTypeName(const Value &value) {
  if (std::holds_alternative<int>(value))
    return "int";

  if (std::holds_alternative<bool>(value))
    return "bool";

  if (std::holds_alternative<double>(value))
    return "float";

  return "Unknown";
}

double getNumericFloat(const Value &value) {
  if (std::holds_alternative<int>(value))
    return static_cast<double>(std::get<int>(value));

  if (std::holds_alternative<double>(value))
    return std::get<double>(value);

  throw std::runtime_error("Expected numeric value");
}

int performIntegerArithmeticOperation(const Value &left, const Value &right,
                                      TokenType op) {
  int leftInt = std::get<int>(left);
  int rightInt = std::get<int>(right);

  switch (op) {
  case TokenType::PLUS:
    return leftInt + rightInt;
  case TokenType::MINUS:
    return leftInt - rightInt;
  case TokenType::STAR:
    return leftInt * rightInt;
  case TokenType::SLASH:
    if (rightInt == 0) {
      throw std::runtime_error("Division by zero");
    }
    return leftInt / rightInt;
  default:
    throw std::runtime_error("Unknown binary operator");
  }
}

float performFloatArithmeticOperation(const Value &left, const Value &right,
                                      TokenType op) {

  double leftFloat = getNumericFloat(left);
  double rightFloat = getNumericFloat(right);

  switch (op) {
  case TokenType::PLUS:
    return leftFloat + rightFloat;
  case TokenType::MINUS:
    return leftFloat - rightFloat;
  case TokenType::STAR:
    return leftFloat * rightFloat;
  case TokenType::SLASH:
    if (rightFloat == 0.0) {
      throw std::runtime_error("Division by zero");
    }
    return leftFloat / rightFloat;
  default:
    throw std::runtime_error("Unknown binary operator");
  }
}

void requireNumericOperands(const Value &left, const Value &right,
                            const std::string &operatorSymbol) {

  if (valueTypeName(left) != "float" && valueTypeName(left) != "int" ||
      valueTypeName(right) != "float" && valueTypeName(right) != "int") {
    throw std::runtime_error("Type error: operator '" + operatorSymbol +
                             "' cannot be applied to " + valueTypeName(left) +
                             " and " + valueTypeName(right));
  }
}

// *****************************************
// Evaluate
// *****************************************

Value Interpreter::evaluate(const Expression *expression) {
  if (auto *integer = dynamic_cast<const IntegerExpression *>(expression)) {
    return integer->value;
  }

  if (auto *identifier =
          dynamic_cast<const IdentifierExpression *>(expression)) {

    return environment.get(identifier->name, identifier->line,
                           identifier->column);
  }

  if (auto *binary = dynamic_cast<const BinaryExpression *>(expression)) {
    auto left = evaluate(binary->left.get());
    auto right = evaluate(binary->right.get());

    switch (binary->operatorType) {
    case TokenType::PLUS: {
      requireNumericOperands(left, right, "+");
      if (valueTypeName(left) == "float" || valueTypeName(right) == "float") {
        return performFloatArithmeticOperation(left, right, TokenType::PLUS);
      }
      return performIntegerArithmeticOperation(left, right, TokenType::PLUS);
    }
    case TokenType::MINUS: {
      requireNumericOperands(left, right, "-");
      if (valueTypeName(left) == "float" || valueTypeName(right) == "float") {
        return performFloatArithmeticOperation(left, right, TokenType::MINUS);
      }
      return performIntegerArithmeticOperation(left, right, TokenType::MINUS);
    }

    case TokenType::STAR: {
      requireNumericOperands(left, right, "*");
      if (valueTypeName(left) == "float" || valueTypeName(right) == "float") {
        return performFloatArithmeticOperation(left, right, TokenType::STAR);
      }
      return performIntegerArithmeticOperation(left, right, TokenType::STAR);
    }

    case TokenType::SLASH: {
      requireNumericOperands(left, right, "/");
      if (valueTypeName(left) == "float" || valueTypeName(right) == "float") {
        return performFloatArithmeticOperation(left, right, TokenType::SLASH);
      }
      return performIntegerArithmeticOperation(left, right, TokenType::SLASH);
    }

    case TokenType::EQUAL_EQUAL:
      if (valueTypeName(left) != valueTypeName(right)) {
        throw std::runtime_error(
            "Type error: operator '==' cannot be applied to " +
            valueTypeName(left) + " and " + valueTypeName(right));
      }
      return left == right;

    case TokenType::NOT_EQUAL:
      if (valueTypeName(left) != valueTypeName(right)) {
        throw std::runtime_error(
            "Type error: operator '!=' cannot be applied to " +
            valueTypeName(left) + " and " + valueTypeName(right));
      }
      return left != right;

    case TokenType::LESS:
      requireNumericOperands(left, right, "<");
      return getNumericFloat(left) < getNumericFloat(right);

    case TokenType::LESS_EQUAL:
      requireNumericOperands(left, right, "<=");
      return getNumericFloat(left) <= getNumericFloat(right);

    case TokenType::GREATER:
      requireNumericOperands(left, right, ">");
      return getNumericFloat(left) > getNumericFloat(right);

    case TokenType::GREATER_EQUAL:
      requireNumericOperands(left, right, ">=");
      return getNumericFloat(left) >= getNumericFloat(right);

    default:
      throw std::runtime_error("Unknown binary operator");
    }
  }

  if (auto *integer = dynamic_cast<const IntegerExpression *>(expression)) {
    return integer->value;
  }

  if (auto *floating = dynamic_cast<const FloatExpression *>(expression)) {
    return floating->value;
  }

  if (auto *boolean = dynamic_cast<const BooleanExpression *>(expression)) {
    return boolean->value;
  }

  throw std::runtime_error("RUNTIME ERROR: Unknown expression");
}

//*****************************************
// Execute statement
//*****************************************
void Interpreter::executeStatement(const Statement *statement) {

  if (auto *declaration =
          dynamic_cast<const VariableDeclaration *>(statement)) {
    Value value = evaluate(declaration->value.get());

    environment.define(declaration->name,
                       Variable{value, declaration->mutable_});
  } else if (auto *assignment =
                 dynamic_cast<const AssignmentStatement *>(statement)) {
    Value value = evaluate(assignment->value.get());

    environment.assign(assignment->name, Variable{value, true},
                       assignment->value->line, assignment->value->column);
  } else if (auto *ifStatement = dynamic_cast<const IfStatement *>(statement)) {
    Value conditionValue = evaluate(ifStatement->condition.get());

    if (!std::holds_alternative<bool>(conditionValue)) {
      throw std::runtime_error(
          "Type error: condition of 'if' statement must be a boolean");
    }

    if (std::get<bool>(conditionValue)) {
      for (const auto &thenStatement : ifStatement->thenBranch) {
        executeStatement(thenStatement.get());
      }
    } else {
      for (const auto &elseStatement : ifStatement->elseBranch) {
        executeStatement(elseStatement.get());
      }
    }
  } else if (auto *print = dynamic_cast<const PrintStatement *>(statement)) {
    Value value = evaluate(print->value.get());

    std::visit(
        [](auto &&value) {
          using T = std::decay_t<decltype(value)>;

          if constexpr (std::is_same_v<T, bool>)
            std::cout << (value ? "true" : "false") << '\n';
          else
            std::cout << value << '\n';
        },
        value);
  } else {
    throw std::runtime_error("RUNTIME_ERROR: Unknown statement");
  }
}

// *****************************************
// Execute
// *****************************************

void Interpreter::execute(const Program &program) {
  for (const auto &statement : program.statements) {
    executeStatement(statement.get());
  }
}
