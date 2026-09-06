---
layout: default
title: Arrays
nav_order: 6
---

Arrays in Vesper are **homogeneous** — every element must share the same type — and can hold values of any supported type, including other arrays.

```ts
let numbers: [int] = [1, 2, 3, 4, 5];

let names: [string] = [
    "Alice",
    "Bob",
    "Charlie"
];
```

## Nested arrays

Arrays can nest to represent multidimensional data, such as matrices:

```ts
let matrix: [[int]] = [
    [1, 2, 3],
    [4, 5, 6]
];
```

Because array types are compositional (see [Types](types.html)), this nesting is unbounded:

```
[int]
[[int]]
[[[float]]]
```

## Indexing

Arrays support indexing with `[...]`:

```ts
let numbers = [10, 20, 30];

print(numbers[0]);
```

Indexing chains naturally for nested arrays:

```ts
let matrix = [
    [1, 2, 3],
    [4, 5, 6]
];

print(matrix[0][1]);
```

Arrays and their element types are checked statically — see [Types](types.html) for how the type checker catches mismatched element types inside an array literal.