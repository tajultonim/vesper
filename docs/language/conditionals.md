---
layout: default
title: Conditionals
parent: Language
nav_order: 6
description: "Use if and else statements to express conditional control flow in Vesper."
---

# Conditionals

Vesper uses `if` and `else` for conditional execution.

## If

```vesper
if (x > 5) {
    print("x is greater than 5");
}
```

The condition must evaluate to a boolean.

## If-else

```vesper
if (x > 5) {
    print("large");
} else {
    print("small");
}
```

## Nested conditionals

Conditionals can be nested.

```vesper
if (x > 0) {
    if (x > 100) {
        print("large positive");
    } else {
        print("positive");
    }
}
```

## Syntax

```text
if (condition) {
    statements
}

if (condition) {
    statements
} else {
    statements
}
```
