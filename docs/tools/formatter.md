---
layout: default
title: Formatter
parent: Tools
nav_order: 2
---

# Formatter

`vspfmt` is the Vesper source formatter.

## Purpose

The formatter provides consistent:

- indentation
- spaces around operators
- function formatting
- block formatting
- array formatting
- comment placement

## Example

Input:

```vesper
fn add(a:int,b:int):int{
let x=a+b;
return x;
}
```

Formatted output:

```vesper
fn add(a: int, b: int): int {
    let x = a + b;
    return x;
}
```

Comments are preserved during formatting.

## Development note

The formatter operates on the parsed AST while using the original token stream to preserve comments that are not represented in the AST.
