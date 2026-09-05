---
layout: default
title: Vesper
description: A small, expressive programming language for learning, experimentation, and scientific computing.
---

# Vesper

> A small, expressive programming language designed for learning, experimentation, and scientific computing.

Vesper is a programming language built from scratch in C++17. It started as an exploration of how programming languages work internally — lexing, parsing, type checking, interpretation — with compilation and native code generation planned for the future.

The long-term goal is to grow Vesper into a language suited for **scientific computing and high-performance computing (HPC)**, while keeping the core language simple and easy to understand.

**Status:** experimental, under active development. Syntax, type system, runtime, and architecture may change substantially. Vesper is currently meant for learning and language-research, not production use.

---

## Features

### Variables

Variables are immutable by default. Use `mut` when mutation is required.

```vesper
let name = "Vesper";
let age: int = 20;

mut counter = 0;
counter = counter + 1;
```

### Type annotations & inference

Types can be written explicitly or left for the type checker to infer.

```vesper
let x: int = 42;
let temperature: float = 36.5;
let enabled: bool = true;
let message: string = "Hello";

// or, inferred:
let x = 42;
let temperature = 36.5;
let message = "Hello";
```

Supported primitive types: `int`, `float`, `bool`, `string`.

### Arrays

Arrays are homogeneous and support arbitrary nesting for multidimensional data.

```vesper
let numbers: [int] = [1, 2, 3, 4, 5];

let matrix: [[int]] = [
    [1, 2, 3],
    [4, 5, 6]
];

print(numbers[0]);
print(matrix[0][1]);
```

Array types compose freely: `[int]`, `[[int]]`, `[[[float]]]`.

### Arithmetic

Alongside the standard operators, Vesper includes operators aimed at numerical work:

```
+     Addition
-     Subtraction
*     Multiplication
/     Division
%     Remainder
//    Floor division
**    Exponentiation
```

Floor division and remainder follow mathematical floor semantics (`-7 // 2 = -4`, `-7 % 2 = 1`), and exponentiation is right-associative (`2 ** 3 ** 2` is `2 ** (3 ** 2)`). Unary minus binds correctly around it too — `-2 ** 2` is `-(2 ** 2)`, while `(-2) ** 2` is `4`.

### Strings, comments, and print

```vesper
let message = "Hello\nWorld";

# line comment
##
block comment
##

print("Value: ", 42, "\n");
```

`print` accepts multiple expressions per call and never appends a newline on its own — that's left to explicit escape sequences.

### Functions

```vesper
fn square(x: float): float {
    x ** 2
}

fn add(a: int, b: int): int {
    a + b
}
```

Functions are expression-oriented: the final expression is the return value. Explicit `return` is planned as the function system matures.

---

## Type system

Vesper's type system is compositional. Primitive types (`int`, `float`, `bool`, `string`) combine into recursive array types (`[int]`, `[[int]]`, `[[[float]]]`) without needing a distinct type per dimension. The type checker validates programs statically before execution — for example, `let numbers: [int] = [1, 2, "three"];` is rejected for mixing incompatible element types.

---

## Architecture

Vesper currently follows a traditional pipeline:

```
Vesper Source → Lexer → Parser → AST → Type Checker → Interpreter
```

The planned long-term architecture splits execution into an interpreter and an independent compiler path, both fed by a shared IR:

```
Vesper Source → Lexer → Parser → AST → Type Checker → Vesper IR
                                                       ↙        ↘
                                            Interpreter        Compiler
                                                                   ↓
                                                             Optimization
                                                                   ↓
                                                            Code Generation
                                                                   ↓
                                                           Native Executable
```

---

## Built with

**Core:** C++17, CMake, GCC / MinGW

**Language implementation:** hand-written lexer, recursive-descent parser, AST, static type checker, tree-walking interpreter

**Developer tools:** `vspfmt` (source formatter), CTest (automated testing)

---

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

---

## Formatting

Vesper ships its own source formatter, `vspfmt`, which operates on the parsed program structure rather than raw text — so it correctly handles expressions, indentation, operators, arrays, indexing, and nested statements.

```bash
# format and print to stdout
./build/vspfmt ./examples/hello.vsp

# format and write back in place
./build/vspfmt ./examples/hello.vsp -w
```

---

## Building

```bash
git clone https://github.com/tajultonim/vesper.git
cd vesper

cmake -S . -B build
cmake --build build

./build/vesper examples/hello.vsp
```

On Windows, use `.\build\vesper.exe .\examples\hello.vsp` instead.

---

## Testing

Vesper uses CTest, currently covering static type-checking:

```bash
cmake --build build
ctest --test-dir build
```

---

## Roadmap

### Language

- [x] Variables & mutable variables (`mut`)
- [x] Type annotations & inference
- [x] Primitive types
- [x] Arrays, including nested arrays and indexing
- [x] Arithmetic, remainder, floor division, exponentiation
- [x] Unary operators
- [x] String escape sequences
- [x] Multi-argument `print`
- [x] Comments (line & block)
- [x] Static type checking
- [x] Source formatter
- [ ] Functions & function calls
- [ ] Explicit `return`
- [ ] `for` loops & `range`
- [ ] Tuples & slicing
- [ ] Structs
- [ ] Pattern matching
- [ ] Generics
- [ ] Modules / imports
- [ ] Standard library

### Compiler

- [ ] Intermediate representation & IR interpreter
- [ ] Constant folding & basic optimizations
- [ ] Native code generation (C backend, RISC-V backend, LLVM backend)
- [ ] AOT and JIT compilation
- [ ] C FFI / C ABI integration

### Scientific computing

- [ ] Matrix and tensor types
- [ ] Linear algebra & numerical methods
- [ ] Scientific standard library
- [ ] SIMD / vectorization
- [ ] Parallel computing & MPI support
- [ ] GPU computing

---

## Why Vesper?

Vesper is primarily a learning project, built to understand programming languages and compilers from the inside out — syntax, lexing, parsing, the AST, the type system, interpretation, IR, optimization, and code generation, in that order. Rather than leaning on a parser generator or compiler framework, the core language is implemented from scratch in C++.

At the same time, it's a testbed for what a small language designed around scientific computing and HPC could look like.

## Long-term vision

The goal is a language that combines the expressiveness of a modern high-level language with the performance and control scientific computing demands:

```
Vesper
 ├── General Programming
 ├── Scientific Computing
 │    ├── Arrays, Matrices, Tensors
 │    └── Numerical Methods
 └── High Performance Computing
      ├── SIMD
      ├── Multithreading
      ├── MPI
      ├── GPU Computing
      └── Native Compilation
```

Development is deliberately staged: the interpreter comes first, as a simple and understandable foundation, before the more involved compiler infrastructure is layered on top.

---

## License

Vesper is released under the [GPL-3.0 License](https://github.com/tajultonim/vesper/blob/main/LICENSE).

## Author

Built by [Tajul Tonim](https://github.com/tajultonim), as an ongoing exploration of programming languages, compilers, numerical computing, and high-performance computing.

[View the repository on GitHub →](https://github.com/tajultonim/vesper)
