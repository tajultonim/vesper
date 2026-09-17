---
layout: default
title: Operators
parent: Language
nav_order: 4
description: "Reference Vesper arithmetic, comparison, unary, division, remainder, and exponentiation operators."
---

# Operators

## Arithmetic operators

| Operator | Meaning |
|---|---|
| `+` | Addition |
| `-` | Subtraction |
| `*` | Multiplication |
| `/` | Division |
| `//` | Integer division |
| `%` | Remainder |
| `**` | Exponentiation |

Examples:

```vesper
let a = 10 + 5;
let b = 10 - 5;
let c = 10 * 5;
let d = 10 / 5;
let e = 10 // 3;
let f = 10 % 3;
let g = 2 ** 8;
```

`/` produces a floating-point result.

`//` produces an integer quotient using the current interpreter's integer
division behavior.

## Comparison operators

| Operator | Meaning |
|---|---|
| `==` | Equal |
| `!=` | Not equal |
| `<` | Less than |
| `>` | Greater than |
| `<=` | Less than or equal |
| `>=` | Greater than or equal |

Comparison expressions produce a `bool`.

```vesper
let x = 10;

let a = x == 10;
let b = x != 5;
let c = x > 3;
let d = x <= 20;
```

## Unary operators

Unary operators operate on a single expression.

```vesper
let x = -10;
```

## Precedence

From lower to higher precedence:

```text
comparison
    ↓
+ -
    ↓
* / % //
    ↓
**
    ↓
unary
    ↓
postfix (calls and indexing)
```

Parentheses can be used to explicitly control evaluation order.

```vesper
let x = (2 + 3) * 4;
```
