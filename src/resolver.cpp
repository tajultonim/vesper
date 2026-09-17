#include "resolver.hpp"

#include <stdexcept>
#include <string>

void Resolver::resolveProgram(const Program &program)
{
    // Global scope
    enterScope();

    for (const auto &statement : program.statements)
    {
        resolveStatement(statement.get());
    }

    leaveScope();
}

void Resolver::resolveStatement(
    const Statement *statement
)
{
    if (const auto *declaration =
            dynamic_cast<const VariableDeclaration *>(statement))
    {
        resolveDeclaration(declaration);
        return;
    }

    if (const auto *assignment =
            dynamic_cast<const AssignmentStatement *>(statement))
    {
        resolveAssignment(assignment);
        return;
    }

    if (const auto *function =
            dynamic_cast<const FunctionStatement *>(statement))
    {
        resolveFunction(function);
        return;
    }

    if (const auto *returnStatement =
            dynamic_cast<const ReturnStatement *>(statement))
    {
        resolveReturn(returnStatement);
        return;
    }

    if (const auto *ifStatement =
            dynamic_cast<const IfStatement *>(statement))
    {
        resolveIf(ifStatement);
        return;
    }

    if (const auto *whileStatement =
            dynamic_cast<const WhileStatement *>(statement))
    {
        resolveWhile(whileStatement);
        return;
    }

    if (const auto *importStatement =
            dynamic_cast<const ImportStatement *>(statement))
    {
        resolveImport(importStatement);
        return;
    }

    if (const auto *expressionStatement =
            dynamic_cast<const ExpressionStatement *>(statement))
    {
        resolveExpression(
            expressionStatement->expression.get()
        );

        return;
    }
}

void Resolver::resolveDeclaration(
    const VariableDeclaration *declaration
)
{
    /*
     * Resolve the initializer before declaring
     * the variable itself.
     *
     * Example:
     *
     * let x = y;
     *
     * y must already exist.
     */
    resolveExpression(
        declaration->value.get()
    );

    declare(
        declaration->name,
        SymbolKind::VARIABLE
    );
}

void Resolver::resolveAssignment(
    const AssignmentStatement *assignment
)
{
    /*
     * The variable being assigned to must already
     * exist somewhere in the current scope chain.
     */
    const Symbol *symbol =
        lookup(assignment->name);

    if (!symbol)
    {
        throw std::runtime_error(
            "RESOLVER:Undefined identifier '" +
            assignment->name +
            "'"
        );
    }

    if (symbol->kind != SymbolKind::VARIABLE &&
        symbol->kind != SymbolKind::PARAMETER)
    {
        throw std::runtime_error(
            "RESOLVER: '" +
            assignment->name +
            "' is not assignable"
        );
    }

    resolveExpression(
        assignment->value.get()
    );
}

void Resolver::resolveFunction(
    const FunctionStatement *function
)
{
    /*
     * The function itself belongs to the
     * surrounding scope.
     */
    declare(
        function->name,
        SymbolKind::FUNCTION
    );

    /*
     * Function parameters and local variables
     * belong to a new function scope.
     */
    enterScope();

    for (const auto &parameter : function->parameters)
    {
        /*
         * Default values can reference names
         * visible from the surrounding scope.
         *
         * Resolve the default expression before
         * declaring the parameter.
         */
        if (parameter.defaultValue)
        {
            resolveExpression(
                parameter.defaultValue.get()
            );
        }

        declare(
            parameter.name,
            SymbolKind::PARAMETER
        );
    }

    /*
     * Resolve function body.
     */
    for (const auto &statement : function->body)
    {
        resolveStatement(statement.get());
    }

    leaveScope();
}

void Resolver::resolveReturn(
    const ReturnStatement *returnStatement
)
{
    if (returnStatement->value)
    {
        resolveExpression(
            returnStatement->value.get()
        );
    }
}

void Resolver::resolveIf(
    const IfStatement *ifStatement
)
{
    /*
     * The condition belongs to the current scope.
     */
    resolveExpression(
        ifStatement->condition.get()
    );

    /*
     * Then branch gets its own scope.
     */
    enterScope();

    for (const auto &statement :
         ifStatement->thenBranch)
    {
        resolveStatement(statement.get());
    }

    leaveScope();

    /*
     * Else branch gets a separate scope.
     */
    if (!ifStatement->elseBranch.empty())
    {
        enterScope();

        for (const auto &statement :
             ifStatement->elseBranch)
        {
            resolveStatement(statement.get());
        }

        leaveScope();
    }
}

void Resolver::resolveWhile(
    const WhileStatement *whileStatement
)
{
    /*
     * The condition belongs to the surrounding scope.
     */
    resolveExpression(
        whileStatement->condition.get()
    );

    /*
     * The loop body gets its own scope.
     */
    enterScope();

    for (const auto &statement :
         whileStatement->body)
    {
        resolveStatement(statement.get());
    }

    leaveScope();
}

void Resolver::resolveImport(
    const ImportStatement *importStatement
)
{
    /*
     * For now, the resolver only registers the
     * imported module alias.
     *
     * Example:
     *
     * import "math" as m;
     *
     * gives:
     *
     * m -> MODULE
     *
     * Actual member resolution:
     *
     * m.square
     *
     * will be implemented once modules have
     * symbol tables.
     */
    declare(
        importStatement->alias,
        SymbolKind::MODULE
    );
}

void Resolver::resolveExpression(
    const Expression *expression
)
{
    if (!expression)
        return;

    /*
     * Identifier
     *
     * Example:
     *
     * x
     */
    if (const auto *identifier =
            dynamic_cast<const IdentifierExpression *>(
                expression))
    {
        resolveIdentifier(identifier);
        return;
    }

    /*
     * Member access
     *
     * Example:
     *
     * m.square
     */
    if (const auto *member =
            dynamic_cast<const MemberExpression *>(
                expression))
    {
        resolveMember(member);
        return;
    }

    /*
     * Function call
     *
     * Example:
     *
     * foo(x)
     */
    if (const auto *call =
            dynamic_cast<const CallExpression *>(
                expression))
    {
        resolveCall(call);
        return;
    }

    /*
     * Binary expression
     *
     * Example:
     *
     * x + y
     */
    if (const auto *binary =
            dynamic_cast<const BinaryExpression *>(
                expression))
    {
        resolveExpression(
            binary->left.get()
        );

        resolveExpression(
            binary->right.get()
        );

        return;
    }

    /*
     * Unary expression
     *
     * Example:
     *
     * -x
     */
    if (const auto *unary =
            dynamic_cast<const UnaryExpression *>(
                expression))
    {
        resolveExpression(
            unary->operand.get()
        );

        return;
    }

    /*
     * Array indexing
     *
     * Example:
     *
     * a[i]
     */
    if (const auto *index =
            dynamic_cast<const IndexExpression *>(
                expression))
    {
        resolveExpression(
            index->object.get()
        );

        resolveExpression(
            index->index.get()
        );

        return;
    }

    /*
     * Array literal
     *
     * Example:
     *
     * [a, b, c]
     */
    if (const auto *array =
            dynamic_cast<const ArrayExpression *>(
                expression))
    {
        for (const auto &element :
             array->elements)
        {
            resolveExpression(
                element.get()
            );
        }

        return;
    }

    /*
     * Literals do not contain names, so there is
     * nothing to resolve.
     *
     * IntegerExpression
     * FloatExpression
     * BooleanExpression
     * StringExpression
     */
}

void Resolver::resolveIdentifier(
    const IdentifierExpression *identifier
)
{
    if (identifier->name == "print" ||
        identifier->name == "input")
    {
        return;
    }

    const Symbol *symbol =
        lookup(identifier->name);

    if (!symbol)
    {
        throw std::runtime_error(
            "RESOLVER: Undefined identifier '" +
            identifier->name +
            "'"
        );
    }

    /*
     * The actual symbol information is currently
     * only used for validation.
     *
     * Later, this is where the AST node can be
     * associated with a Symbol / SymbolId.
     */
}

void Resolver::resolveMember(
    const MemberExpression *member
)
{
    /*
     * First resolve the object.
     *
     * For:
     *
     * m.square
     *
     * this resolves:
     *
     * m -> MODULE
     */
    resolveExpression(
        member->object.get()
    );

    /*
     * We don't resolve the actual member yet.
     *
     * Later:
     *
     * m
     * ↓
     * math module
     * ↓
     * square
     * ↓
     * FunctionSymbol
     */
}

void Resolver::resolveCall(
    const CallExpression *call
)
{
    /*
     * Resolve the function / method being called.
     *
     * Examples:
     *
     * foo()
     * m.square()
     * array.method()
     */
    resolveExpression(
        call->callee.get()
    );

    /*
     * Resolve every argument.
     */
    for (const auto &argument :
         call->arguments)
    {
        resolveExpression(
            argument.get()
        );
    }
}

void Resolver::enterScope()
{
    scopes.emplace_back();
}

void Resolver::leaveScope()
{
    if (scopes.empty())
    {
        throw std::runtime_error(
            "RESOLVER: no scope to leave"
        );
    }

    scopes.pop_back();
}

void Resolver::declare(
    const std::string &name,
    SymbolKind kind
)
{
    if (scopes.empty())
    {
        throw std::runtime_error(
            "RESOLVER: no active scope"
        );
    }

    Scope &currentScope =
        scopes.back();

    /*
     * Duplicate declaration in the SAME scope
     * is an error.
     *
     * Shadowing an outer scope is allowed.
     */
    if (currentScope.find(name) != currentScope.end())
    {
        throw std::runtime_error(
            "RESOLVER: Duplicate declaration of '" +
            name +
            "'"
        );
    }

    currentScope.emplace(
        name,
        Symbol{
            name,
            kind
        }
    );
}

const Symbol *Resolver::lookup(
    const std::string &name
) const
{
    /*
     * Search from innermost scope toward
     * outermost scope.
     */
    for (auto it = scopes.rbegin();
         it != scopes.rend();
         ++it)
    {
        const Scope &scope = *it;

        auto symbol =
            scope.find(name);

        if (symbol != scope.end())
        {
            return &symbol->second;
        }
    }

    return nullptr;
}