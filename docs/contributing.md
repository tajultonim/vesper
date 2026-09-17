---
layout: default
title: Contributing
nav_order: 5
description: "Learn how to build, test, document, and contribute to the Vesper programming language project."
---

# Contributing

Vesper is an evolving programming-language project.

## Source tree

The current implementation is organized approximately as:

```text
vesper/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── token.hpp
│   ├── token.cpp
│   ├── lexer.hpp
│   ├── lexer.cpp
│   ├── ast.hpp
│   ├── ast.cpp
│   ├── parser.hpp
│   ├── parser.cpp
│   ├── environment.hpp
│   ├── environment.cpp
│   ├── interpreter.hpp
│   ├── interpreter.cpp
│   ├── type_checker.hpp
│   ├── type_checker.cpp
│   └── type.hpp
├── tests/
└── tools/
    ├── formatter.hpp
    ├── formatter.cpp
    └── vspfmt.cpp
```

## Build

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

## Design direction

The long-term compiler architecture is planned around:

```text
Vesper source
    ↓
Lexer / Parser
    ↓
AST
    ↓
Static Type System
    ↓
Vesper IR
    ↓
Vesper-specific optimizations
    ↓
LLVM IR
    ↓
LLVM
    ↓
Native code
```

Potential future areas include:

- native compilation
- LLVM backend
- improved numeric types
- ownership and memory-model features
- efficient multidimensional arrays
- SIMD
- parallel loops
- GPU computing
- C interoperability
- scientific-computing libraries

Features should only be marked as stable in the language documentation after they are implemented and tested.
