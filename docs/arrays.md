---
layout: default
title: Arrays
permalink: /arrays/
---

**[Home](/) · [Installation](/installation/) · [Building & Testing](/building/) · [Variables](/variables/) · [Types](/types/) · [Arrays](/arrays/) · [Operators](/operators/) · [Functions](/functions/) · [Architecture](/architecture/) · [Roadmap](/roadmap/) · [Contributing](/contributing/)**

---

# Arrays

Arrays in Vesper are **homogeneous** — every element must share the same type — and can hold values of any supported type, including other arrays.

```vesper
let numbers: [int] = [1, 2, 3, 4, 5];

let names: [string] = [
    "Alice",
    "Bob",
    "Charlie"
];
```

## Nested arrays

Arrays can nest to represent multidimensional data, such as matrices:

```vesper
let matrix: [[int]] = [
    [1, 2, 3],
    [4, 5, 6]
];
```

Because array types are compositional (see [Types](/types/)), this nesting is unbounded:

```
[int]
[[int]]
[[[float]]]
```

## Indexing

Arrays support indexing with `[...]`:

```vesper
let numbers = [10, 20, 30];

print(numbers[0]);
```

Indexing chains naturally for nested arrays:

```vesper
let matrix = [
    [1, 2, 3],
    [4, 5, 6]
];

print(matrix[0][1]);
```

Arrays and their element types are checked statically — see [Types](/types/) for how the type checker catches mismatched element types inside an array literal.
