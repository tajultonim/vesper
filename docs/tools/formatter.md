---
layout: default
title: Formatter
parent: Tools
nav_order: 2
description: "Format Vesper source with vspfmt while preserving comments and current language syntax."
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

The formatter uses a recovery parser for recoverable syntax issues, such as a
missing semicolon, and reports the parser diagnostic before emitting the
normalized source. The interpreter remains strict and will refuse to execute
the same invalid source.

The formatter also supports imports, module member calls, arrays, indexing,
`export` and `extern` functions, `void` return types, expression statements,
and bare `return;` statements.

## Development note

The formatter operates on the parsed AST while using the original token stream to preserve comments that are not represented in the AST.
