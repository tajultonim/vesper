---
layout: default
title: Scope
parent: Language
nav_order: 10
description: "Understand lexical scope, nested environments, shadowing, and name resolution in Vesper."
---

# Scope

Vesper uses lexical environments to resolve variables.

## Block scope

Variables declared inside a block are local to that block.

```vesper
if (true) {
    let x = 10;
    print(x);
}
```

The implementation creates nested environments for function calls and block execution.

## Function scope

Function parameters and local variables belong to the function's environment.

```vesper
fn add(a: int, b: int): int {
    let result = a + b;
    return result;
}
```

`a`, `b`, and `result` are local to the function.

## Mutation

A variable declared with `mut` may be reassigned within its valid scope.

```vesper
mut x = 10;
x = 20;
```

An immutable `let` binding cannot be reassigned.

```vesper
let x = 10;
x = 20; # Type/checking error
```

## Global access

Global-variable visibility inside functions should follow the current scope implementation. As the compiler evolves, this section should be expanded to document the exact lexical-scope and shadowing rules.
