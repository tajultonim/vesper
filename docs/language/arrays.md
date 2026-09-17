---
layout: default
title: Arrays
parent: Language
nav_order: 9
description: "Create homogeneous Vesper arrays, nested arrays, and indexed data structures."
---

# Arrays

Vesper supports array literals and array indexing.

## Array literals

An array is written as a comma-separated list inside square brackets.

```vesper
let numbers = [1, 2, 3, 4];
```

Elements are expressions.

```vesper
let a = 10;
let numbers = [a, a + 1, a + 2];
```

## Typed arrays

An array type uses `[type]`.

```vesper
let numbers: [int] = [1, 2, 3];
let values: [float] = [1.0, 2.0, 3.0];
```

## Indexing

```vesper
let numbers: [int] = [10, 20, 30];

let first = numbers[0];
let second = numbers[1];
```

## Nested arrays

Because array types are recursive, nested arrays can be represented.

```vesper
let matrix: [[int]] = [
    [1, 2],
    [3, 4]
];
```

> Array bounds checking and mutation semantics should be documented here as the array implementation evolves.
