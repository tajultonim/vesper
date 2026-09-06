#include <cmath>
#include <iostream>
#include <sstream>
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

  if (std::holds_alternative<std::string>(value))
    return "string";

  return "Unknown";
}

double getNumericFloat(const Value &value) {
  if (std::holds_alternative<int>(value))
    return static_cast<double>(std::get<int>(value));

  if (std::holds_alternative<double>(value))
    return std::get<double>(value);

  throw std::runtime_error("RUNTIME ERROR: Expected numeric value");
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

  case TokenType::PERCENT: {
    if (rightInt == 0)
      throw std::runtime_error("RUNTIME ERROR: Division by zero");

    return leftInt % rightInt;
  }

  case TokenType::STAR_STAR:
    if (leftInt == 0 && rightInt < 0) {
      throw std::runtime_error(
          "RUNTIME ERROR: 0 cannot be raised to a negative power");
    }
    return static_cast<int>(std::pow(leftInt, rightInt));

  default:
    throw std::runtime_error("RUNTIME ERROR: Unknown binary operator");
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
  case TokenType::STAR_STAR:
    return std::pow(leftFloat, rightFloat);
  case TokenType::SLASH:
    if (rightFloat == 0.0) {
      throw std::runtime_error("RUNTIME ERROR: Division by zero");
    }
    return leftFloat / rightFloat;

  default:
    throw std::runtime_error("RUNTIME ERROR: Unknown binary operator");
  }
}

void requireNumericOperands(const Value &left, const Value &right,
                            const std::string &operatorSymbol) {

  if (valueTypeName(left) != "float" && valueTypeName(left) != "int" ||
      valueTypeName(right) != "float" && valueTypeName(right) != "int") {
    throw std::runtime_error("RUNTIME ERROR: operator '" + operatorSymbol +
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

  if (auto *unary = dynamic_cast<const UnaryExpression *>(expression)) {
    Value operandValue = evaluate(unary->operand.get());

    switch (unary->operatorType) {
    case TokenType::MINUS:
      if (valueTypeName(operandValue) == "int") {
        return -std::get<int>(operandValue);
      } else if (valueTypeName(operandValue) == "float") {
        return -std::get<double>(operandValue);
      } else {
        throw std::runtime_error(
            "RUNTIME ERROR: unary '-' operator requires numeric operand");
      }

    case TokenType::PLUS:
      if (valueTypeName(operandValue) == "int") {
        return std::get<int>(operandValue);
      } else if (valueTypeName(operandValue) == "float") {
        return std::get<double>(operandValue);
      } else {
        throw std::runtime_error(
            "RUNTIME ERROR: unary '+' operator requires numeric operand");
      }

    default:
      throw std::runtime_error("RUNTIME ERROR: Unknown unary operator");
    }
  }

  if (auto *binary = dynamic_cast<const BinaryExpression *>(expression)) {
    auto left = evaluate(binary->left.get());
    auto right = evaluate(binary->right.get());

    switch (binary->operatorType) {
    case TokenType::PLUS: {

      if (valueTypeName(left) == "string" && valueTypeName(right) == "string") {
        return std::get<std::string>(left) + std::get<std::string>(right);
      }
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

      if (valueTypeName(left) == "string" && valueTypeName(right) == "int") {
        std::string result;
        for (int i = 0; i < std::get<int>(right); ++i) {
          result += std::get<std::string>(left);
        }
        return result;
      }

      requireNumericOperands(left, right, "*");
      if (valueTypeName(left) == "float" || valueTypeName(right) == "float") {
        return performFloatArithmeticOperation(left, right, TokenType::STAR);
      }
      return performIntegerArithmeticOperation(left, right, TokenType::STAR);
    }

    case TokenType::SLASH: {
      return performFloatArithmeticOperation(left, right, TokenType::SLASH);
    }

    case TokenType::STAR_STAR: {
      requireNumericOperands(left, right, "**");
      if (valueTypeName(left) == "float" || valueTypeName(right) == "float" ||
          std::get<int>(right) < 0) {
        return performFloatArithmeticOperation(left, right,
                                               TokenType::STAR_STAR);
      }
      return performIntegerArithmeticOperation(left, right,
                                               TokenType::STAR_STAR);
    }

    case TokenType::SLASH_SLASH: {
      if (getNumericFloat(right) == 0.0) {
        throw std::runtime_error("RUNTIME ERROR: Division by zero");
      }

      return static_cast<int>(getNumericFloat(left) / getNumericFloat(right));
    }

    case TokenType::PERCENT: {
      requireNumericOperands(left, right, "%");
      return performIntegerArithmeticOperation(left, right, TokenType::PERCENT);
    }

    case TokenType::EQUAL_EQUAL:
      if (valueTypeName(left) != valueTypeName(right)) {
        throw std::runtime_error(
            "RUNTIME ERROR: operator '==' cannot be applied to " +
            valueTypeName(left) + " and " + valueTypeName(right));
      }
      return left == right;

    case TokenType::NOT_EQUAL:
      if (valueTypeName(left) != valueTypeName(right)) {
        throw std::runtime_error(
            "RUNTIME ERROR: operator '!=' cannot be applied to " +
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
      throw std::runtime_error("RUNTIME ERROR: Unknown binary operator");
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

  if (auto *string = dynamic_cast<const StringExpression *>(expression)) {
    return string->value;
  }

  if (auto *array = dynamic_cast<const ArrayExpression *>(expression)) {
    auto result = std::make_shared<Array>();
    for (const auto &element : array->elements) {
      result->elements.push_back(evaluate(element.get()));
    }
    return result;
  }

  if (auto *index = dynamic_cast<const IndexExpression *>(expression)) {
    Value objectValue = evaluate(index->object.get());
    Value indexValue = evaluate(index->index.get());

    if (!std::holds_alternative<std::shared_ptr<Array>>(objectValue)) {
      throw std::runtime_error("RUNTIME ERROR: cannot index a non-array value");
    }

    if (!std::holds_alternative<int>(indexValue)) {
      throw std::runtime_error("RUNTIME ERROR: array index must be an integer");
    }

    auto arrayPtr = std::get<std::shared_ptr<Array>>(objectValue);
    int idx = std::get<int>(indexValue);

    if (idx < 0 || idx >= static_cast<int>(arrayPtr->elements.size())) {
      throw std::runtime_error("RUNTIME ERROR: Index out of bounds");
    }

    return arrayPtr->elements[idx];
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
          "RUNTIME ERROR: condition of 'if' statement must be a boolean");
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

  } else if (auto *whileStatement =
                 dynamic_cast<const WhileStatement *>(statement)) {
    while (true) {
      Value conditionValue = evaluate(whileStatement->condition.get());

      if (!std::holds_alternative<bool>(conditionValue)) {
        throw std::runtime_error(
            "RUNTIME ERROR: condition of 'while' statement must be a boolean");
      }

      if (!std::get<bool>(conditionValue)) {
        break;
      }

      for (const auto &bodyStatement : whileStatement->body) {
        executeStatement(bodyStatement.get());
      }
    }

  } else if (auto *print = dynamic_cast<const PrintStatement *>(statement)) {
    for (const auto &expression : print->values) {
      Value value = evaluate(expression.get());

      std::visit(
          [](auto &&value) {
            using T = std::decay_t<decltype(value)>;

            if constexpr (std::is_same_v<T, bool>)
              std::cout << (value ? "true" : "false");
            else if constexpr (std::is_same_v<T, double>) {
              std::ostringstream stream;
              stream << value;

              std::string output = stream.str();

              if (output.find('.') == std::string::npos &&
                  output.find('e') == std::string::npos &&
                  output.find('E') == std::string::npos) {
                output += ".0";
              }

              std::cout << output;
            } else
              std::cout << value;
          },
          value);
    }
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
