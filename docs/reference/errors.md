---
layout: default
title: Errors
parent: Reference
nav_order: 4
---

# Errors

Vesper reports errors from several stages of execution.

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
file:line:column: error: message
```
