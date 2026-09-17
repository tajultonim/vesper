---
layout: default
title: Variables
parent: Language
nav_order: 2
description: "Declare immutable and mutable Vesper variables with type inference and optional annotations."
---

# Variables

Vesper has immutable variables by default.

## Immutable variables

Use `let` to declare an immutable variable.

```vesper
let x: int = 10;
```

An immutable variable cannot be reassigned.

```vesper
let x: int = 10;
x = 20; # Error
```

## Mutable variables

Use `mut` when a variable must be changed.

```vesper
mut x: int = 10;

x = 20;
```

## Type inference

The type annotation can be omitted when the type can be inferred from the initializer.

```vesper
let x = 10;
let temperature = 25.5;
let enabled = true;
let name = "Vesper";
```

The inferred types are:

```text
x           → int
temperature → float
enabled     → bool
name        → string
```

## Explicit type annotations

Types can be specified explicitly.

```vesper
let count: int = 100;
let pi: float = 3.14159;
let ready: bool = true;
let name: string = "Vesper";
```

The initializer must be compatible with the declared type.

## Syntax

```text
let identifier [: type] = expression;
mut identifier [: type] = expression;
```
