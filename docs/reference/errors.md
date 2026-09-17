---
layout: default
title: Errors
parent: Reference
nav_order: 4
description: "Understand Vesper CLI, lexer, parser, module, resolver, type-checking, and runtime errors."
---

# Errors

Vesper reports errors from several stages of execution.

The implementation uses typed errors derived from `VesperError`:

| Error type | Stage |
|---|---|
| `CliError` | command-line arguments and file I/O |
| `LexerError` | source tokenization |
| `ParserError` | AST construction |
| `ModuleError` | module resolution, loading, and cycles |
| `ResolverError` | name and scope resolution |
| `TypeError` | static type checking |
| `RuntimeError` | interpretation and evaluation |

The CLI catches these errors at the pipeline boundary and reports the phase,
source file, and any line/column information already available in the
diagnostic.

## Lexical errors

These occur when the lexer cannot recognize source text.

Examples include malformed literals or invalid characters.

## Parse errors

These occur when source text does not match Vesper's grammar.

Example:

```vesper
let x = ;
```

## Type errors

These occur when expressions or statements violate the static type system.

Example:

```vesper
let x: int = "hello";
```

## Mutability errors

Attempting to assign to an immutable binding is invalid.

```vesper
let x = 10;
x = 20;
```

## Function-call errors

The checker/interpreter can reject invalid function calls, including incompatible argument types, missing required arguments, or too many arguments.

## Error locations

Vesper tracks source line and column information for tokens and statements. This allows diagnostics to identify where an error occurred.

As the compiler matures, error messages should consistently follow:

```text
[PHASE] file:line:column: error: message
```

For pipeline diagnostics, run the interpreter with `--verbose` or `-V`.

The interpreter uses strict parsing: a parser error stops execution before
resolution, type checking, or interpretation. The formatter uses recovery
parsing for recoverable delimiter errors so it can normalize source such as a
missing semicolon. Errors are color-coded by phase when printed in a terminal.
