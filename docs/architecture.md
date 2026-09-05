---
layout: default
title: Architecture
nav_order: 9
---

## Current pipeline

Vesper currently follows a traditional language-processing pipeline:

```
Vesper Source
      │
      ▼
   Lexer
      │
      ▼
   Parser
      │
      ▼
     AST
      │
      ▼
Type Checker
      │
      ▼
 Interpreter
```

Source text is tokenized by a hand-written lexer, parsed by a recursive-descent parser into an AST, statically validated by the type checker, and then executed directly by a tree-walking interpreter.

## Planned architecture

The long-term plan splits execution into two paths — an interpreter for a reference execution model, and an independently evolving compiler — both fed by a shared intermediate representation (IR):

```
     Vesper Source
           │
           ▼
        Lexer
           │
           ▼
        Parser
           │
           ▼
          AST
           │
           ▼
     Type Checker
           │
           ▼
        Vesper IR
         /     \
        /       \
       ▼         ▼
Interpreter   Compiler
                 │
                 ▼
           Optimization
                 │
                 ▼
           Code Generation
                 │
                 ▼
         Native Executable
```

## Project structure

```
vesper/
├── CMakeLists.txt
│
├── src/
│   ├── main.cpp
│   ├── token.hpp / token.cpp
│   ├── lexer.hpp / lexer.cpp
│   ├── ast.hpp / ast.cpp
│   ├── parser.hpp / parser.cpp
│   ├── environment.hpp / environment.cpp
│   ├── interpreter.hpp / interpreter.cpp
│   ├── type.hpp
│   └── type_checker.hpp / type_checker.cpp
│
├── tests/
│   └── type_checker_test.cpp
│
├── tools/
│   ├── formatter.hpp / formatter.cpp
│   └── vspfmt.cpp
│
└── examples/
    └── *.vsp
```

## Why this shape?

Vesper is deliberately staged: the interpreter is built first, as a simple and understandable foundation, with the more involved compiler infrastructure — IR, optimization passes, code generation, native backends — layered on afterward. The interpreter continues to serve as a reference execution model even once the compiler exists.

At the same time, this pipeline is a lens into how language implementations work in general — syntax, lexing, parsing, the AST, the type system, interpretation, IR, optimization, and code generation, in that order. That's the underlying motivation for the project: understanding programming languages and compilers from the inside out, built from scratch in C++ rather than relying on a parser generator or compiler framework.

See the [Roadmap](roadmap.html) for what's planned on top of this architecture, including scientific computing and HPC features like SIMD, multithreading, MPI, and GPU computing.