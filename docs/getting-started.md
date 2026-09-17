---
layout: default
title: Getting Started
nav_order: 3
description: "Run Vesper programs, format source files, use the command-line tools, and understand the interpreter workflow."
---

# Getting Started

## Source files

Vesper programs use the `.vsp` extension.

```text
hello.vsp
```

## Running a program

The command-line interface is:

```bash
vesper <file.vsp>
```

For example:

```bash
vesper hello.vsp
```

## Command-line options

```bash
vesper --help
vesper -h

vesper --version
vesper -v
```

The version shown by the executable is available with `vesper --version`.

## Formatter

Vesper includes a formatter named `vspfmt`.

```bash
vspfmt <file.vsp>
```

To overwrite the file with formatted value use:

```bash

vspfmt <file.vsp> -w

```


The formatter is intended to produce consistent indentation, spacing, and layout while preserving comments.

## VS Code extension

The Vesper VS Code extension provides syntax highlighting, autocomplete,
formatting, and a Run button for `.vsp` files. The extension does not require
the interpreter or formatter binaries to be bundled. Configure their paths or
make `vesper` and `vspfmt` available on `PATH`.

See the [VS Code guide](tools/vscode.md) for the complete lookup order.

## Typical development flow

```text
write .vsp
    ↓
lexer
    ↓
parser
    ↓
AST
    ↓
type checker
    ↓
interpreter
```
