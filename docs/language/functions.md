---
layout: default
title: Functions
parent: Language
nav_order: 8
---

# Functions

Functions are declared with the `fn` keyword.

## Function declaration

```vesper
fn add(a: int, b: int): int {
    return a + b;
}
```

A function declaration contains:

1. `fn`
2. function name
3. parameter list
4. return type
5. function body

## Syntax

```text
fn name(parameters): return_type {
    statements
}
```

## Parameters

Each parameter has a name and a type.

```vesper
fn multiply(a: int, b: int): int {
    return a * b;
}
```

## Return type

The return type follows `:`.

```vesper
fn square(x: float): float {
    return x ** 2;
}
```

## Return

Vesper uses an explicit `return` statement.

```vesper
fn add(a: int, b: int): int {
    return a + b;
}
```

## Calling functions

```vesper
let result = add(10, 20);
```

## Multiple arguments

```vesper
fn sum3(a: int, b: int, c: int): int {
    return a + b + c;
}

let result = sum3(1, 2, 3);
```

## Default parameters

Parameters can have default values.

```vesper
fn power(x: float, exponent: int = 2): float {
    return x ** exponent;
}
```

The default is used when the argument is omitted.

```vesper
let square = power(5.0);
let cube = power(5.0, 3);
```

## Recursion

Functions can call themselves.

```vesper
fn factorial(n: int): int {
    if (n <= 1) {
        return 1;
    }

    return n * factorial(n - 1);
}

let result = factorial(5);
```

The language implementation supports recursive function calls.

## Forward calls

Function declarations are checked in a separate pass from function bodies, allowing calls to functions that are declared later in the source program.

```vesper
let result = later();

fn later(): int {
    return 42;
}
```

> The exact behavior of top-level execution and initialization should follow the current interpreter/compiler implementation.
