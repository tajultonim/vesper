---
layout: default
title: Command Line
parent: Tools
nav_order: 1
---

# Command Line

The Vesper executable is invoked with a `.vsp` source file.

```text
vesper <file.vsp>
```

## Verbose diagnostics

Use `--verbose` or `-V` to print lexer, parser, module-loading, name-resolution,
type-checking, and execution diagnostics to standard error. Verbose mode does
not print the full token list.

```text
vesper --verbose program.vsp
vesper -V program.vsp
```

## Token dump

Use `--tokens` to print the complete token list to standard output:

```text
vesper program.vsp --tokens
```

Provide a path after `--tokens` to write the token list to a file:

```text
vesper --tokens tokens.txt program.vsp
vesper --verbose --tokens=tokens.txt program.vsp
```

The token file contains each token's type, source value, line, and column.

The program's normal output remains on standard output, so verbose diagnostics
can be redirected separately:

```text
vesper --verbose program.vsp > output.txt 2> debug.log
```

## Help

```text
vesper --help
vesper -h
```

## Version

```text
vesper --version
vesper -v
```

The version is reported by the executable:

```text
vesper --version
```

## Module paths

Imports are resolved relative to the importing file. These declarations are
equivalent when `physics.vsp` and `math.vsp` are siblings:

```vesper
import "math" as math;
import "./math" as math;
```

The loader caches each resolved module and detects circular dependencies.

Interpreter errors are color-coded by phase and include the source file and
line/column when available.
