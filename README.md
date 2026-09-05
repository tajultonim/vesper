<h1 align="center">
    <img src="https://github.com/tajultonim/vesper/blob/main/icons/vesper-animated.svg" width="70"/><br/>
    VESPER
</h1>

> A small, expressive programming language designed for learning, experimentation, and scientific computing.

**Vesper** is a programming language I'm building from scratch in C++. The project started as an exploration of how programming languages work internally-from lexing and parsing to type checking and interpretation, with compilation and native code generation planned for the future.

The long-term goal is to evolve Vesper into a language particularly suited for **scientific computing and high-performance computing (HPC)** while keeping the language simple, expressive, and easy to understand.

---

## ✨ Features

Vesper is currently under active development.

### Variables

Variables are immutable by default. Use `mut` when mutation is required.

```ts
let name = "Vesper";
let age: int = 20;

mut counter = 0;
counter = counter + 1;
```

### Type Annotations & Type Inference

Vesper supports optional type annotations and type inference.

```ts
let x: int = 42;
let temperature: float = 36.5;
let enabled: bool = true;
let message: string = "Hello";
```

Types can be omitted when they can be inferred:

```ts
let x = 42;
let temperature = 36.5;
let message = "Hello";
```

---

### Primitive Types

Vesper currently supports:

```text
int
float
bool
string
```

---

### Arrays

Arrays are homogeneous and can contain values of any supported type.

```ts
let numbers: [int] = [1, 2, 3, 4, 5];

let names: [string] = [
    "Alice",
    "Bob",
    "Charlie"
];
```

Nested arrays are supported, allowing multidimensional data structures:

```ts
let matrix: [[int]] = [
    [1, 2, 3],
    [4, 5, 6]
];
```

Array types are compositional, so arbitrary nesting is possible:

```text
[int]
[[int]]
[[[float]]]
```

---

### Array Indexing

Arrays can be accessed using indexing.

```ts
let numbers = [10, 20, 30];

print(numbers[0]);
```

Indexing can also be chained for nested arrays:

```ts
let matrix = [
    [1, 2, 3],
    [4, 5, 6]
];

print(matrix[0][1]);
```

---

### Arithmetic

Vesper supports standard arithmetic operators as well as operators useful for numerical computing.

```text
+     Addition
-     Subtraction
*     Multiplication
/     Division
%     Remainder
//    Floor division
**    Exponentiation
```

Examples:

```ts
let a = 10 % 3;
let b = 10 // 3;
let c = 2 ** 10;
let d = 2 ** -2;
```

Floor division follows mathematical floor semantics:

```text
-7 // 2 = -4
```

The corresponding remainder is:

```text
-7 % 2 = 1
```

Exponentiation is right-associative:

```ts
2 ** 3 ** 2
```

is interpreted as:

```text
2 ** (3 ** 2)
```

---

### Unary Operators

Vesper supports unary `+` and `-` for numeric values.

```ts
let x = -42;
let y = +10;
let z = -3.14;
```

Exponentiation follows mathematical precedence:

```ts
-2 ** 2
```

is interpreted as:

```text
-(2 ** 2)
```

while:

```ts
(-2) ** 2
```

is interpreted as:

```text
4
```

---

### Strings & Escape Sequences

Vesper supports escape sequences in strings.

```ts
let message = "Hello\nWorld";
let path = "C:\\Users\\Vesper";
```

Supported escape sequences are being expanded as the language develops.

---

### Print

`print` supports multiple expressions in a single statement.

```ts
print("Value: ", 42, "\n");
```

Each expression is evaluated and written sequentially.

`print` does **not** automatically append a newline. Newlines can be explicitly produced using string escape sequences:

```ts
print("Hello\n");
print("World\n");
```

---

### Comments

Vesper uses `#` for line comments.

```ts
# This is a comment

let x = 10; # Inline comment
```

Block comments are also supported by the language tooling:

```ts
##
This is a block comment.
##
```

The use of `#` avoids a conflict with Vesper's floor-division operator `//`.

---

### Functions

Vesper uses a concise syntax with typed parameters and an optional typed return value.

```ts
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

Vesper is designed around **expression-oriented functions**, where the final expression can serve as the function result.

Explicit `return` support is planned as the function system develops.

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

The type checker performs static validation before execution.

For example:

```ts
let numbers: [int] = [1, 2, "three"];
```

is rejected because the array contains values of incompatible types.

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

The interpreter will provide a reference execution model while the compiler evolves independently.

---

## 🔧 Built With

Vesper is currently implemented in **C++17**.

### Core

* C++17
* CMake
* GCC / MinGW

### Language Implementation

* Hand-written lexer
* Recursive-descent parser
* Abstract Syntax Tree (AST)
* Static type checker
* Tree-walking interpreter

### Developer Tools

* `vspfmt` - Vesper source formatter
* CTest - automated testing

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
│   ├── type.hpp
│   ├── type_checker.hpp
│   └── type_checker.cpp
│
├── tests/
│   └── type_checker_test.cpp
│
├── tools/
│   ├── formatter.hpp
│   ├── formatter.cpp
│   └── vspfmt.cpp
│
└── examples/
    └── *.vsp
```

---

## 🎨 Formatting

Vesper includes its own source formatter, `vspfmt`.

Format a source file:

```powershell
.\build\vspfmt.exe .\examples\hello.vsp
```

Format and write the result back to the file:

```powershell
.\build\vspfmt.exe .\examples\hello.vsp -w
```

The formatter operates on the parsed program structure rather than simply manipulating source text, allowing it to correctly handle expressions, indentation, operators, arrays, indexing, and nested statements.

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

Or:

```bash
cmake --build build
ctest --test-dir build
```

The test suite currently includes static type-checking tests.

---

## 🎯 Roadmap

Vesper is still in its early stages and is being developed incrementally.

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
* [x] Remainder (`%`)
* [x] Floor division (`//`)
* [x] Exponentiation (`**`)
* [x] Unary operators
* [x] String escape sequences
* [x] Multi-argument `print`
* [x] Comments
* [x] Static type checking
* [x] Source formatter
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
* [ ] Modules / imports
* [ ] Standard library

### Compiler

* [ ] Intermediate representation
* [ ] IR interpreter
* [ ] Constant folding
* [ ] Basic optimizations
* [ ] Native code generation
* [ ] C backend
* [ ] RISC-V backend
* [ ] LLVM backend
* [ ] AOT compilation
* [ ] JIT compilation
* [ ] C FFI / C ABI integration

### Scientific Computing

The long-term goal is to make Vesper useful for numerical and scientific workloads.

Planned areas include:

* [ ] Matrix types
* [ ] Tensor types
* [ ] Linear algebra
* [ ] Numerical methods
* [ ] Scientific standard library
* [ ] SIMD / vectorization
* [ ] Parallel computing
* [ ] MPI support
* [ ] GPU computing

---

## 🔬 Why Vesper?

Vesper is primarily a **learning project**, but it is being designed with a larger goal in mind.

The project is an attempt to understand programming languages and compilers from the inside out:

```text
Syntax
  ↓
Lexing
  ↓
Parsing
  ↓
AST
  ↓
Type System
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

At the same time, Vesper explores what a small language designed around **scientific computing and HPC** could look like.

Rather than relying on a parser generator or compiler framework for the core language, the language implementation is being built from the ground up using C++.

---

## 🔭 Long-Term Vision

The long-term goal is for Vesper to combine the expressiveness of a modern high-level language with the performance and control required for scientific computing.

Possible areas include:

```text
Vesper
  │
  ├── General Programming
  │
  ├── Scientific Computing
  │     ├── Arrays
  │     ├── Matrices
  │     ├── Tensors
  │     └── Numerical Methods
  │
  └── High Performance Computing
        ├── SIMD
        ├── Multithreading
        ├── MPI
        ├── GPU Computing
        └── Native Compilation
```

The language is intentionally being developed in small stages, with the interpreter providing a simple and understandable foundation before more sophisticated compiler infrastructure is introduced.

---

## 📚 Project Status

**Vesper is experimental and under active development.**

The language syntax, type system, runtime, standard library, and compiler architecture may change substantially as development continues.

It is currently intended for **learning, experimentation, and language-development research**, rather than production use.

---

## 📄 License

Vesper is released under the **GPL3 License**.

See [`LICENSE`](LICENSE) for details.

---

## 👤 Author

**Tajul Tonim**

Built as an ongoing exploration of programming languages, compilers, numerical computing, and high-performance computing.
