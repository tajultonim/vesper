---
layout: default
title: Getting Started
nav_order: 2
---

# Getting Started

## Source files

Vesper programs use the `.vsp` extension.

```text
hello.vsp
```

## Running a program

The command-line interface is:

```text
vesper <file.vsp>
```

For example:

```text
vesper hello.vsp
```

## Command-line options

```text
vesper --help
vesper -h

vesper --version
vesper -v
```

The current development version is `0.1.0`.

## Formatter

Vesper includes a formatter named `vspfmt`.

```text
vspfmt <file.vsp>
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
