---
layout: default
title: Operators
nav_order: 7
---

## Arithmetic

Alongside the standard operators, Vesper includes a few aimed squarely at numerical work:

```
+     Addition
-     Subtraction
*     Multiplication
/     Division
%     Remainder
//    Floor division
**    Exponentiation
```

```vesper
let a = 10 % 3;
let b = 10 // 3;
let c = 2 ** 10;
let d = 2 ** -2;
```

**Floor division** follows mathematical floor semantics rather than truncation:

```
-7 // 2 = -4
```

and the corresponding **remainder** is:

```
-7 % 2 = 1
```

**Exponentiation** is right-associative, so:

```
2 ** 3 ** 2
```

is interpreted as `2 ** (3 ** 2)`.

## Unary operators

Vesper supports unary `+` and `-` for numeric values:

```vesper
let x = -42;
let y = +10;
let z = -3.14;
```

Unary minus respects mathematical precedence around exponentiation:

```
-2 ** 2
```

is interpreted as `-(2 ** 2)`, while:

```
(-2) ** 2
```

is `4`.

## Strings & escape sequences

Strings support escape sequences:

```vesper
let message = "Hello\nWorld";
let path = "C:\\Users\\Vesper";
```

More escape sequences are being added as the language develops.

## Print

`print` accepts multiple expressions in a single call, evaluating and writing each in sequence:

```vesper
print("Value: ", 42, "\n");
```

It does **not** automatically append a newline — produce one explicitly with `\n` when you want it:

```vesper
print("Hello\n");
print("World\n");
```

## Comments

Line comments use `#`:

```vesper
# This is a comment

let x = 10; # Inline comment
```

Block comments are supported by the language tooling using a doubled `##`:

```vesper
##
This is a block comment.
##
```

`#` was chosen deliberately so it doesn't collide with Vesper's floor-division operator, `//`.