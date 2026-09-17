---
layout: default
title: Operators
parent: Reference
nav_order: 3
description: "Vesper operator precedence and behavior reference."
---

# Operator Reference

| Precedence | Operators | Category |
|---:|---|---|
| 1 | `== != < > <= >=` | Comparison |
| 2 | `+ -` | Addition / subtraction |
| 3 | `* / % //` | Multiplication / division |
| 4 | `**` | Exponentiation |
| 5 | unary operators | Unary |
| 6 | `()` `[]` | Call / indexing |

Higher precedence binds more tightly.

For example:

```vesper
let x = 2 + 3 * 4;
```

is evaluated as:

```text
2 + (3 * 4)
```

Parentheses override normal precedence:

```vesper
let x = (2 + 3) * 4;
```
