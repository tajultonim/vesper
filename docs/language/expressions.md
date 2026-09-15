---
layout: default
title: Expressions
parent: Language
nav_order: 5
---

# Expressions

Expressions produce values.

## Literals

### Integer

```vesper
42
```

### Float

```vesper
3.14159
```

### Boolean

```vesper
true
false
```

### String

```vesper
"Hello"
```

## Identifiers

Variables can be referenced by name.

```vesper
let x = 10;
let y = x + 5;
```

## Binary expressions

```vesper
let result = a + b * 2;
```

## Function calls

A function call consists of a function expression followed by arguments in parentheses.

```vesper
let result = add(10, 20);
```

Arguments are separated by commas.

```vesper
print(x, y, "done");
```

## Array indexing

An array element can be accessed with `[]`.

```vesper
let values: [int] = [10, 20, 30];

let x = values[0];
```

Index expressions can themselves be expressions.

```vesper
let i = 1;
let x = values[i];
```

## Expression statements

An expression can appear as a statement, for example a function call:

```vesper
print("Hello");
```
