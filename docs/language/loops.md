---
layout: default
title: Loops
parent: Language
nav_order: 7
description: "Write while loops and nested iteration in Vesper programs."
---

# Loops

Vesper supports `while` loops.

## While

```vesper
mut i: int = 0;

while (i < 10) {
    print(i);
    i = i + 1;
}
```

The condition is evaluated before each iteration.

## Syntax

```text
while (condition) {
    statements
}
```

The loop terminates when the condition becomes `false`.

## Nested loops

Loops can be nested.

```vesper
mut i: int = 0;

while (i < 3) {
    mut j: int = 0;

    while (j < 3) {
        print(i, j);
        j = j + 1;
    }

    i = i + 1;
}
```

> `for` loops are planned language functionality and should not be considered part of the current stable syntax unless implemented in your current compiler/interpreter.
