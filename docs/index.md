---
layout: default
title: Vesper
nav_order: 1
description: "Vesper programming language documentation."
permalink: /
---

# Vesper

**Vesper** is a statically typed programming language designed for scientific computing, high-performance computing, and systems-oriented software.

Its syntax is intended to remain simple while providing strong foundations for numerical programming and future compiler optimizations.

## Example

```vesper
fn factorial(n: int): int {
    if (n <= 1) {
        return 1;
    }

    return n * factorial(n - 1);
}

let result = factorial(10);
print(result);
```

## Documentation

- [Installation](installation.md)
- [Getting Started](getting-started.md)
- [Language](language/index.md)
- [Reference](reference/index.md)
- [Tools](tools/index.md)

## Current implementation

The current Vesper implementation includes a lexer, parser, AST, static type checker, tree-walk interpreter, and formatter.

It also supports recursive modules with imports resolved relative to the file
that declares each import. The formatter is available as `vspfmt`, and the
Vesper VS Code extension provides syntax highlighting, autocomplete, formatting,
and a Run command.

The native compiler and LLVM backend are future development goals.
