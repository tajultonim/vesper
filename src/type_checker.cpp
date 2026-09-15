#include "type_checker.hpp"

#include <stdexcept>
#include <memory>
#include <string>
#include <utility>


bool TypeChecker::sameType(
    const Type &left,
    const Type &right
) const
{
    if (left.kind != right.kind)
        return false;

    if (left.kind == Type::Kind::ARRAY)
    {
        if (!left.elementType || !right.elementType)
            return false;

        return sameType(
            *left.elementType,
            *right.elementType
        );
    }

    return true;
}


bool TypeChecker::isNumeric(
    const Type &type
) const
{
    return type.kind == Type::Kind::INT ||
           type.kind == Type::Kind::FLOAT;
}


Type TypeChecker::checkExpression(
    const Expression *expression
)
{
    if (auto integer =
            dynamic_cast<const IntegerExpression *>(expression))
    {
        return Type(Type::Kind::INT);
    }


    if (auto floating =
            dynamic_cast<const FloatExpression *>(expression))
    {
        return Type(Type::Kind::FLOAT);
    }


    if (auto boolean =
            dynamic_cast<const BooleanExpression *>(expression))
    {
        return Type(Type::Kind::BOOL);
    }


    if (auto string =
            dynamic_cast<const StringExpression *>(expression))
    {
        return Type(Type::Kind::STRING);
    }


    if (auto identifier =
            dynamic_cast<const IdentifierExpression *>(expression))
    {
        auto it = types.find(identifier->name);

        if (it == types.end())
        {
            throw std::runtime_error(
                "Undefined variable '" +
                identifier->name +
                "' at line " +
                std::to_string(identifier->line) +
                ", column " +
                std::to_string(identifier->column)
            );
        }

        return it->second.type;
    }


    if (auto array =
            dynamic_cast<const ArrayExpression *>(expression))
    {
        if (array->elements.empty())
        {
            throw std::runtime_error(
                "Cannot infer type of empty array"
            );
        }

        Type elementType =
            checkExpression(array->elements[0].get());

        for (std::size_t i = 1;
             i < array->elements.size();
             ++i)
        {
            Type currentType =
                checkExpression(array->elements[i].get());

            if (!sameType(elementType, currentType))
            {
                throw std::runtime_error(
                    "Array elements must have the same type"
                );
            }
        }

        return Type(
            Type::Kind::ARRAY,
            std::make_unique<Type>(elementType)
        );
    }


    if (auto index =
            dynamic_cast<const IndexExpression *>(expression))
    {
        Type objectType =
            checkExpression(index->object.get());

        Type indexType =
            checkExpression(index->index.get());

        if (objectType.kind != Type::Kind::ARRAY)
        {
            throw std::runtime_error(
                "Indexing requires an array"
            );
        }

        if (indexType.kind != Type::Kind::INT)
        {
            throw std::runtime_error(
                "Array index must be an integer"
            );
        }

        if (!objectType.elementType)
        {
            throw std::runtime_error(
                "Array has no element type"
            );
        }

        return *objectType.elementType;
    }


    if (auto unary =
            dynamic_cast<const UnaryExpression *>(expression))
    {
        Type operandType =
            checkExpression(unary->operand.get());

        if (unary->operatorType == TokenType::PLUS ||
            unary->operatorType == TokenType::MINUS)
        {
            if (!isNumeric(operandType))
            {
                throw std::runtime_error(
                    "Unary operator requires a numeric operand"
                );
            }

            return operandType;
        }

        throw std::runtime_error(
            "Unknown unary operator"
        );
    }


    if (auto binary =
            dynamic_cast<const BinaryExpression *>(expression))
    {
        Type leftType =
            checkExpression(binary->left.get());

        Type rightType =
            checkExpression(binary->right.get());


        switch (binary->operatorType)
        {
        case TokenType::PLUS:

            if (leftType.kind == Type::Kind::STRING &&
                rightType.kind == Type::Kind::STRING)
            {
                return Type(Type::Kind::STRING);
            }

            if (!isNumeric(leftType) ||
                !isNumeric(rightType))
            {
                throw std::runtime_error(
                    "Operator '+' requires numeric operands "
                    "or two strings"
                );
            }

            if (leftType.kind == Type::Kind::FLOAT ||
                rightType.kind == Type::Kind::FLOAT)
            {
                return Type(Type::Kind::FLOAT);
            }

            return Type(Type::Kind::INT);


        case TokenType::MINUS:

            if (!isNumeric(leftType) ||
                !isNumeric(rightType))
            {
                throw std::runtime_error(
                    "Operator '-' requires numeric operands"
                );
            }

            if (leftType.kind == Type::Kind::FLOAT ||
                rightType.kind == Type::Kind::FLOAT)
            {
                return Type(Type::Kind::FLOAT);
            }

            return Type(Type::Kind::INT);


        case TokenType::STAR:

            // string * int
            if (leftType.kind == Type::Kind::STRING &&
                rightType.kind == Type::Kind::INT)
            {
                return Type(Type::Kind::STRING);
            }

            if (!isNumeric(leftType) ||
                !isNumeric(rightType))
            {
                throw std::runtime_error(
                    "Operator '*' requires numeric operands"
                );
            }

            if (leftType.kind == Type::Kind::FLOAT ||
                rightType.kind == Type::Kind::FLOAT)
            {
                return Type(Type::Kind::FLOAT);
            }

            return Type(Type::Kind::INT);


        case TokenType::SLASH:

            if (!isNumeric(leftType) ||
                !isNumeric(rightType))
            {
                throw std::runtime_error(
                    "Operator '/' requires numeric operands"
                );
            }

            // Vesper '/' always produces float.
            return Type(Type::Kind::FLOAT);


        case TokenType::SLASH_SLASH:

            if (!isNumeric(leftType) ||
                !isNumeric(rightType))
            {
                throw std::runtime_error(
                    "Operator '//' requires numeric operands"
                );
            }

            return Type(Type::Kind::INT);


        case TokenType::PERCENT:

            if (leftType.kind != Type::Kind::INT ||
                rightType.kind != Type::Kind::INT)
            {
                throw std::runtime_error(
                    "Operator '%' requires integer operands"
                );
            }

            return Type(Type::Kind::INT);


        case TokenType::STAR_STAR:

            if (!isNumeric(leftType) ||
                !isNumeric(rightType))
            {
                throw std::runtime_error(
                    "Operator '**' requires numeric operands"
                );
            }

            if (leftType.kind == Type::Kind::FLOAT ||
                rightType.kind == Type::Kind::FLOAT)
            {
                return Type(Type::Kind::FLOAT);
            }

            return Type(Type::Kind::INT);


        case TokenType::LESS:
        case TokenType::LESS_EQUAL:
        case TokenType::GREATER:
        case TokenType::GREATER_EQUAL:

            if (!isNumeric(leftType) ||
                !isNumeric(rightType))
            {
                throw std::runtime_error(
                    "Comparison requires numeric operands"
                );
            }

            return Type(Type::Kind::BOOL);


        case TokenType::EQUAL_EQUAL:
        case TokenType::NOT_EQUAL:

            if (!sameType(leftType, rightType))
            {
                throw std::runtime_error(
                    "Equality comparison requires operands "
                    "of the same type"
                );
            }

            return Type(Type::Kind::BOOL);


        default:
            throw std::runtime_error(
                "Unknown binary operator"
            );
        }
    }


    if (auto call =
            dynamic_cast<const CallExpression *>(expression))
    {
        return checkCall(call);
    }


    throw std::runtime_error(
        "Unknown expression"
    );
}


Type TypeChecker::checkCall(
    const CallExpression *call
)
{
    auto identifier =
        dynamic_cast<const IdentifierExpression *>(
            call->callee.get()
        );

    if (!identifier)
    {
        throw std::runtime_error(
            "Function calls currently require "
            "a function name"
        );
    }

    auto functionIt =
        functions.find(identifier->name);

    if (functionIt == functions.end())
    {
        throw std::runtime_error(
            "Undefined function '" +
            identifier->name +
            "'"
        );
    }

    const FunctionInfo &function =
        functionIt->second;

    const std::size_t argumentCount =
        call->arguments.size();

    const std::size_t parameterCount =
        function.parameterTypes.size();


    if (argumentCount > parameterCount)
    {
        throw std::runtime_error(
            "Too many arguments in call to function '" +
            identifier->name +
            "'"
        );
    }


    for (std::size_t i = 0;
         i < argumentCount;
         ++i)
    {
        Type argumentType =
            checkExpression(call->arguments[i].get());

        const Type &parameterType =
            function.parameterTypes[i];

        if (!sameType(argumentType, parameterType))
        {
            throw std::runtime_error(
                "Argument " +
                std::to_string(i + 1) +
                " of function '" +
                identifier->name +
                "' has incorrect type"
            );
        }
    }


    for (std::size_t i = argumentCount;
         i < parameterCount;
         ++i)
    {
        if (!function.hasDefault[i])
        {
            throw std::runtime_error(
                "Missing argument for parameter " +
                std::to_string(i + 1) +
                " of function '" +
                identifier->name +
                "'"
            );
        }
    }


    return function.returnType;
}


void TypeChecker::checkDeclaration(
    const VariableDeclaration *declaration
)
{
    Type expressionType =
        checkExpression(declaration->value.get());


    if (declaration->declaredType)
    {
        if (!sameType(
                *declaration->declaredType,
                expressionType
            ))
        {
            throw std::runtime_error(
                "Initializer type does not match "
                "declared variable type"
            );
        }
    }


    Type finalType =
        declaration->declaredType
            ? *declaration->declaredType
            : expressionType;


    types.insert_or_assign(
        declaration->name,
        VariableInfo{
            std::move(finalType),
            declaration->mutable_
        }
    );
}


void TypeChecker::checkAssignment(
    const AssignmentStatement *assignment
)
{
    auto it = types.find(assignment->name);

    if (it == types.end())
    {
        throw std::runtime_error(
            "Undefined variable '" +
            assignment->name +
            "'"
        );
    }


    if (!it->second.mutable_)
    {
        throw std::runtime_error(
            "Cannot assign to immutable variable '" +
            assignment->name +
            "'"
        );
    }


    Type valueType =
        checkExpression(assignment->value.get());


    if (!sameType(
            it->second.type,
            valueType
        ))
    {
        throw std::runtime_error(
            "Assigned value has incorrect type"
        );
    }
}


void TypeChecker::checkIfStatement(
    const IfStatement *ifStatement
)
{
    Type conditionType =
        checkExpression(ifStatement->condition.get());

    if (conditionType.kind != Type::Kind::BOOL)
    {
        throw std::runtime_error(
            "If condition must be boolean"
        );
    }


    for (const auto &statement :
         ifStatement->thenBranch)
    {
        checkStatement(statement.get());
    }


    for (const auto &statement :
         ifStatement->elseBranch)
    {
        checkStatement(statement.get());
    }
}


void TypeChecker::checkWhileStatement(
    const WhileStatement *whileStatement
)
{
    Type conditionType =
        checkExpression(whileStatement->condition.get());

    if (conditionType.kind != Type::Kind::BOOL)
    {
        throw std::runtime_error(
            "While condition must be boolean"
        );
    }


    for (const auto &statement :
         whileStatement->body)
    {
        checkStatement(statement.get());
    }
}


void TypeChecker::checkFunction(
    const FunctionStatement *function
)
{
    // Save the current scope.
    auto previousTypes =
        std::move(types);

    auto previousReturnType =
        currentFunctionReturnType;


    // Start function-local scope.
    types.clear();


    // Parameters are immutable variables.
    for (const auto &parameter :
         function->parameters)
    {
        types.insert_or_assign(
            parameter.name,
            VariableInfo{
                parameter.type,
                false
            }
        );
    }


    currentFunctionReturnType =
        &function->returnType;


    for (const auto &statement :
         function->body)
    {
        checkStatement(statement.get());
    }


    // Restore outer scope.
    currentFunctionReturnType =
        previousReturnType;

    types =
        std::move(previousTypes);
}


void TypeChecker::checkReturn(
    const ReturnStatement *returnStatement
)
{
    if (currentFunctionReturnType == nullptr)
    {
        throw std::runtime_error(
            "Return statement outside of a function"
        );
    }


    Type returnType =
        checkExpression(
            returnStatement->value.get()
        );


    if (!sameType(
            returnType,
            *currentFunctionReturnType
        ))
    {
        throw std::runtime_error(
            "Return type does not match "
            "function return type"
        );
    }
}


void TypeChecker::checkStatement(
    const Statement *statement
)
{
    if (auto declaration =
            dynamic_cast<const VariableDeclaration *>(
                statement
            ))
    {
        checkDeclaration(declaration);
        return;
    }


    if (auto assignment =
            dynamic_cast<const AssignmentStatement *>(
                statement
            ))
    {
        checkAssignment(assignment);
        return;
    }


    if (auto ifStatement =
            dynamic_cast<const IfStatement *>(
                statement
            ))
    {
        checkIfStatement(ifStatement);
        return;
    }


    if (auto whileStatement =
            dynamic_cast<const WhileStatement *>(
                statement
            ))
    {
        checkWhileStatement(whileStatement);
        return;
    }


    if (auto function =
            dynamic_cast<const FunctionStatement *>(
                statement
            ))
    {
        checkFunction(function);
        return;
    }


    if (auto returnStatement =
            dynamic_cast<const ReturnStatement *>(
                statement
            ))
    {
        checkReturn(returnStatement);
        return;
    }


    if (auto expressionStatement =
            dynamic_cast<const ExpressionStatement *>(
                statement
            ))
    {
        checkExpression(
            expressionStatement->expression.get()
        );
        return;
    }


    if (auto print =
            dynamic_cast<const PrintStatement *>(
                statement
            ))
    {
        for (const auto &value :
             print->values)
        {
            checkExpression(value.get());
        }

        return;
    }


    throw std::runtime_error(
        "Unknown statement"
    );
}


void TypeChecker::checkProgram(
    const Program &program
)
{
    /*
     * Pass 1:
     *
     * Register every function signature before checking
     * any function body.
     *
     * This is required for recursive functions such as:
     *
     * fn factorial(n: int): int {
     *     return factorial(n - 1);
     * }
     */
    for (const auto &statement :
         program.statements)
    {
        auto function =
            dynamic_cast<const FunctionStatement *>(
                statement.get()
            );

        if (!function)
            continue;


        if (functions.find(function->name) !=
            functions.end())
        {
            throw std::runtime_error(
                "Function '" +
                function->name +
                "' is already defined"
            );
        }


        FunctionInfo info{
            {},
            {},
            function->returnType
        };


        for (const auto &parameter :
             function->parameters)
        {
            info.parameterTypes.push_back(
                parameter.type
            );

            info.hasDefault.push_back(
                parameter.defaultValue != nullptr
            );
        }


        functions.emplace(
            function->name,
            std::move(info)
        );
    }


    /*
     * Pass 2:
     *
     * Now check all statements.
     *
     * Function calls can safely refer to functions that
     * appeared earlier or later in the source file.
     */
    for (const auto &statement :
         program.statements)
    {
        checkStatement(statement.get());
    }
}