---
layout: default
title: Functions
nav_order: 8
---

Vesper uses a concise function syntax with typed parameters and an optional typed return value:

```vesper
fn square(x: float): float {
    x ** 2
}

fn add(a: int, b: int): int {
    a + b
}

fn hello(): string {
    "Hello"
}
```

## Expression-oriented

Functions in Vesper are **expression-oriented** — the final expression in the function body is automatically the function's result, with no `return` keyword required. In `square`, `x ** 2` is that final expression and becomes the returned value.

Explicit `return` support, along with general function calls beyond what's already wired into the interpreter, is planned as the function system develops further — see the [Roadmap](roadmap.html) for where this sits relative to other in-progress work.