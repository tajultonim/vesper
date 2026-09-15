---
layout: default
title: Getting Started
nav_order: 3
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

The current development version is `0.1.0`.

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
