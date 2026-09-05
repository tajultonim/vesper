# Vesper

> A small, expressive programming language designed for learning, experimentation, and scientific computing.

**Vesper** is a programming language I'm building from scratch in C++. The project started as an exploration of how programming languages work internally—from lexing and parsing to type checking, interpretation, and eventually compilation to native code.

The long-term goal is to evolve Vesper into a language that is particularly well suited for **scientific computing and high-performance computing (HPC)** while keeping the language simple and expressive.

---

## ✨ Features

Vesper is currently under active development.

### Variables

Variables are immutable by default, with `mut` available for mutable variables.

```vesper
let name = "Vesper"
let age: int = 20

mut counter = 0
counter = counter + 1
```

### Type Annotations

Vesper supports optional type annotations and type inference.

```vesper
let x: int = 42
let temperature: float = 36.5
let enabled: bool = true
let message: string = "Hello"
```

### Functions

Functions use a concise syntax with typed parameters and return values.

```vesper
fn square(x: float): float {
    x ** 2
}

fn add(a: int, b: int): int {
    a + b
}

fn hello(): string {
    "Hello"
}
```

### Arrays

Arrays can contain values of any supported type.

```vesper
let numbers: [int] = [1, 2, 3, 4, 5]

let names: [string] = [
    "Alice",
    "Bob",
    "Charlie"
]
```

Nested arrays are supported, allowing multidimensional data structures:

```vesper
let matrix: [[int]] = [
    [1, 2, 3],
    [4, 5, 6]
]
```

### Array Indexing

Arrays can be indexed and chained:

```vesper
numbers[0]

matrix[0][1]
```

### Arithmetic

Vesper supports standard arithmetic operators as well as operators useful for numerical computing.

```text
+    Addition
-    Subtraction
*    Multiplication
/    Division
%    Remainder
//   Floor division
**   Exponentiation
```

Examples:

```vesper
let a = 10 % 3
let b = 10 // 3
let c = 2 ** 10
let d = 2 ** -2
```

Exponentiation is right-associative:

```vesper
2 ** 3 ** 2
```

is interpreted as:

```text
2 ** (3 ** 2)
```

### Unary Operators

Unary operators are supported for numeric values.

```vesper
let x = -42
let y = +10
```

Exponentiation follows mathematical precedence:

```vesper
-2 ** 2
```

is interpreted as:

```text
-(2 ** 2)
```

---

## 🧠 Type System

Vesper uses a compositional type system.

Currently supported primitive types include:

```text
int
float
bool
string
```

Array types are represented recursively:

```text
[int]
[[int]]
[[[float]]]
```

This allows multidimensional structures without introducing a separate type for every dimension.

The type checker catches errors before execution.

For example:

```vesper
let numbers: [int] = [1, 2, "three"]
```

is rejected because all elements of an array must have the same type.

---

## 🏗️ Architecture

Vesper currently follows a traditional language-processing pipeline:

```text
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

The long-term architecture is planned to become:

```text
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

The interpreter will serve as a reference implementation while the compiler evolves independently.

---

## 🔧 Built With

Vesper is currently implemented in **C++17**.

### Core

* C++17
* CMake
* MinGW / GCC

### Compiler Components

* Hand-written lexer
* Recursive-descent parser
* Abstract Syntax Tree (AST)
* Static type checker
* Tree-walking interpreter

---

## 📁 Project Structure

```text
vesper/
├── CMakeLists.txt
│
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
│   ├── value.hpp
│   ├── type.hpp
│   ├── type_checker.hpp
│   └── type_checker.cpp
│
├── tests/
│   └── type_checker_test.cpp
│
└── tools/
    └── vesperfmt.cpp
```

---

## 🚀 Building

Clone the repository:

```bash
git clone https://github.com/tajultonim/vesper.git
cd vesper
```

Configure the project:

```bash
cmake -S . -B build
```

Build:

```bash
cmake --build build
```

The Vesper executable will be generated inside `build/`.

Run a Vesper program:

```bash
./build/vesper examples/hello.vsp
```

On Windows:

```powershell
.\build\vesper.exe .\examples\hello.vsp
```

---

## 🧪 Testing

Vesper uses CTest for its test suite.

```bash
ctest --test-dir build
```

Or build and run the tests through CMake:

```bash
cmake --build build
ctest --test-dir build
```

---

## 🎯 Roadmap

Vesper is still in its early stages.

### Language

* [x] Variables
* [x] Mutable variables
* [x] Type annotations
* [x] Type inference
* [x] Primitive types
* [x] Arrays
* [x] Nested arrays
* [x] Array indexing
* [x] Arithmetic operators
* [x] Floor division
* [x] Exponentiation
* [x] Unary operators
* [x] Static type checking
* [ ] Functions
* [ ] Function calls
* [ ] Explicit `return`
* [ ] `for` loops
* [ ] `range`
* [ ] Tuples
* [ ] Slicing
* [ ] Structs
* [ ] Pattern matching
* [ ] Generics
* [ ] Standard library

### Compiler

* [ ] Intermediate representation
* [ ] IR interpreter
* [ ] Constant folding
* [ ] Basic optimizations
* [ ] Native code generation
* [ ] RISC-V backend
* [ ] LLVM backend
* [ ] AOT compilation
* [ ] JIT compilation

### Scientific Computing

The long-term goal is to make Vesper useful for numerical and scientific workloads.

Planned areas include:

* [ ] Matrix types
* [ ] Tensor types
* [ ] Linear algebra
* [ ] Numerical methods
* [ ] Scientific standard library
* [ ] SIMD/vectorization
* [ ] Parallel computing
* [ ] MPI support
* [ ] GPU computing

---

## 🔬 Why Vesper?

Vesper is primarily a **learning project**, but it is being designed with a larger goal in mind.

I want to understand programming languages from the inside out:

```text
Syntax
  ↓
Lexing
  ↓
Parsing
  ↓
AST
  ↓
Type Systems
  ↓
Interpretation
  ↓
Intermediate Representation
  ↓
Optimization
  ↓
Code Generation
  ↓
Machine Code
```

At the same time, Vesper is an experiment in what a small language designed around **scientific computing and HPC** could look like.

The project is intentionally being built from the ground up rather than relying on a parser generator or compiler framework for the core language implementation.

---

## 📚 Project Status

**Vesper is experimental and under active development.**

The language syntax, type system, standard library, and compiler architecture may change substantially as the project develops.

It is currently suitable for experimentation and learning rather than production use.

---

## 📄 License

Vesper is released under the **MIT License**.

See [`LICENSE`](LICENSE) for details.

---

## 👤 Author

**Tajul Tonim**

Built as an ongoing exploration of programming languages, compilers, numerical computing, and high-performance computing.
