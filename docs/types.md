---
layout: default
title: Types
permalink: /types/
---

**[Home](/) · [Installation](/installation/) · [Building & Testing](/building/) · [Variables](/variables/) · [Types](/types/) · [Arrays](/arrays/) · [Operators](/operators/) · [Functions](/functions/) · [Architecture](/architecture/) · [Roadmap](/roadmap/) · [Contributing](/contributing/)**

---

# Types

## Type annotations & inference

Vesper supports optional type annotations along with type inference — write out a type when it helps readability, or leave it off and let the type checker figure it out from the initializer.

```vesper
let x: int = 42;
let temperature: float = 36.5;
let enabled: bool = true;
let message: string = "Hello";
```

Types can be omitted when they're inferable from context:

```vesper
let x = 42;
let temperature = 36.5;
let message = "Hello";
```

## Primitive types

Vesper currently supports four primitive types:

```
int
float
bool
string
```

## Compositional type system

Beyond the primitives, Vesper's type system is **compositional**: array types are built recursively out of other types, rather than needing a distinct named type for every dimension.

```
[int]
[[int]]
[[[float]]]
```

This is what lets Vesper express multidimensional data — matrices, nested lists, and so on — without a proliferation of special-cased types. See [Arrays](/arrays/) for how these types are used in practice.

## Static type checking

The type checker validates a program statically, before it runs. This catches type mismatches — including inside array literals — ahead of execution:

```vesper
let numbers: [int] = [1, 2, "three"];
```

This is rejected, because the array literal mixes an `int`/`int` pair with a `string`, and `[int]` requires every element to be an `int`.
