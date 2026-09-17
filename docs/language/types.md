---
layout: default
title: Types
parent: Language
nav_order: 3
---

# Types

Vesper uses static types.

## Primitive types

| Type | Description |
|---|---|
| `int` | Integer values |
| `float` | Floating-point values |
| `bool` | Boolean values |
| `string` | Text values |
| `void` | No return value |

### Integer

```vesper
let n: int = 42;
```

### Float

```vesper
let x: float = 3.14;
```

### Boolean

```vesper
let active: bool = true;
let finished: bool = false;
```

### String

```vesper
let message: string = "Hello, Vesper";
```

## Array types

Arrays are written using square brackets around the element type.

```vesper
let numbers: [int] = [1, 2, 3, 4];
let values: [float] = [1.0, 2.5, 3.75];
```

Nested arrays can be represented recursively.

```vesper
let matrix: [[float]] = [[1.0, 2.0], [3.0, 4.0]];
```

## Type inference

Vesper can infer a variable's type from its initializer.

```vesper
let n = 42;
let x = 3.14;
let ok = true;
let text = "hello";
```

## Static checking

The type checker detects incompatible operations before interpretation.

```vesper
let x: int = 10;
let y: string = "hello";

# Invalid: incompatible operand types
let z = x + y;
```
