#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "ast.hpp"
#include "type.hpp"


struct VariableInfo
{
    Type type;
    bool mutable_;
};


struct FunctionInfo
{
    std::vector<Type> parameterTypes;
    std::vector<bool> hasDefault;
    Type returnType;
};


class TypeChecker
{
public:
    void checkProgram(const Program &program);


private:
    // Expressions
    Type checkExpression(const Expression *expression);
    Type checkCall(const CallExpression *call);

    // Statements
    void checkStatement(const Statement *statement);

    void checkDeclaration(
        const VariableDeclaration *declaration
    );

    void checkAssignment(
        const AssignmentStatement *assignment
    );

    void checkIfStatement(
        const IfStatement *ifStatement
    );

    void checkWhileStatement(
        const WhileStatement *whileStatement
    );

    void checkFunction(
        const FunctionStatement *function
    );

    void checkReturn(
        const ReturnStatement *returnStatement
    );


    // Type utilities
    bool sameType(
        const Type &left,
        const Type &right
    ) const;

    bool isNumeric(
        const Type &type
    ) const;


    // Variables currently visible in the type-checking scope
    std::unordered_map<std::string, VariableInfo> types;

    // Function signatures
    std::unordered_map<std::string, FunctionInfo> functions;

    // Return type of the function currently being checked.
    // nullptr means we are not inside a function.
    const Type *currentFunctionReturnType = nullptr;
};