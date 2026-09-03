# Vesper

> A small programming language and interpreter built from scratch in C++.

Vesper is an experimental programming language designed and implemented from the ground up to explore how programming languages work internally - from **lexing and parsing to AST construction and interpretation**.

The project is being developed incrementally, with the long-term goal of evolving Vesper into a more complete compiled language.

## 🚧 Project Status

**Early development - experimental**

The current implementation already supports:

* Lexical analysis
* Recursive-descent parsing
* Abstract Syntax Trees (AST)
* Integer expressions
* Arithmetic operators
* Operator precedence
* Parenthesized expressions
* Variables
* Variable references
* `let` declarations
* `print(...)`
* Expression evaluation
* Runtime environment

More language features and a compiler backend are planned.

---

## Example

A simple Vesper program:

```vesper
let x = 10;
let y = x + 20;

print(x);
print(y);
print(x * y);
```

Output:

```text
10
30
300
```

Expressions support standard arithmetic precedence:

```vesper
let result = 10 + 20 * 3;

print(result);
```

Output:

```text
70
```

Parentheses can be used to explicitly control evaluation:

```vesper
let result = (10 + 20) * 3;

print(result);
```

Output:

```text
90
```

---

## Architecture

Vesper currently follows a traditional interpreter pipeline:

```text
Source Code
    │
    ▼
  Lexer
    │
    ▼
  Tokens
    │
    ▼
  Parser
    │
    ▼
   AST
    │
    ▼
Interpreter
    │
    ▼
Environment
    │
    ▼
  Output
```

### Lexer

The lexer converts source code into a sequence of tokens.

For example:

```vesper
let x = 10 + 20;
```

becomes conceptually:

```text
LET
IDENTIFIER
EQUAL
INTEGER
PLUS
INTEGER
SEMICOLON
END_OF_FILE
```

### Parser

Vesper uses a **recursive-descent parser**.

Expression precedence is represented by the parser hierarchy:

```text
parseExpression()
      │
      ├── + -
      │
      ▼
parseMultiplication()
      │
      ├── * /
      │
      ▼
parsePrimary()
      │
      ├── integers
      ├── identifiers
      └── (...)
```

This allows:

```vesper
10 + 20 * 3
```

to produce an AST equivalent to:

```text
      +
     / \
   10   *
       / \
      20  3
```

rather than:

```text
      *
     / \
    +   3
   / \
 10  20
```

### Abstract Syntax Tree

The parser constructs an AST using C++ classes and `std::unique_ptr`.

Current expression hierarchy:

```text
Expression
├── IntegerExpression
├── IdentifierExpression
└── BinaryExpression
```

Statement hierarchy:

```text
Statement
├── LetStatement
└── PrintStatement
```

A complete program contains a collection of statements.

### Interpreter

The interpreter recursively evaluates the AST.

For:

```vesper
x + 20
```

the interpreter:

1. Looks up `x` in the environment.
2. Evaluates `20`.
3. Applies `+`.
4. Returns the resulting integer.

The runtime environment currently stores variables as name/value pairs.

---

## Project Structure

```text
vesper/
├── CMakeLists.txt
└── src/
    ├── main.cpp
    ├── token.hpp
    ├── lexer.hpp
    ├── lexer.cpp
    ├── ast.hpp
    ├── ast.cpp
    ├── parser.hpp
    ├── parser.cpp
    ├── environment.hpp
    ├── environment.cpp
    ├── interpreter.hpp
    └── interpreter.cpp
```

### Components

| File            | Purpose                                |
| --------------- | -------------------------------------- |
| `token.hpp`     | Token types and token representation   |
| `lexer.*`       | Source-code tokenization               |
| `ast.*`         | AST node definitions and AST utilities |
| `parser.*`      | Recursive-descent parser               |
| `environment.*` | Runtime variable storage               |
| `interpreter.*` | AST evaluation and execution           |
| `main.cpp`      | Program entry point                    |

---

## Building

### Requirements

* C++17-compatible compiler
* GCC, Clang, or MSVC
* CMake *(optional; direct compilation is currently possible)*

### GCC

From the project root:

```bash
g++ src/main.cpp \
    src/lexer.cpp \
    src/parser.cpp \
    src/ast.cpp \
    src/environment.cpp \
    src/interpreter.cpp \
    -o vesper
```

On Windows:

```powershell
g++ src/main.cpp src/lexer.cpp src/parser.cpp src/ast.cpp src/environment.cpp src/interpreter.cpp -o vesper
```

Run:

```powershell
.\vesper.exe
```

---

## Current Language

### Variables

Variables are declared with `let`:

```vesper
let x = 10;
let y = x + 20;
```

### Arithmetic

Supported operators:

```text
+   addition
-   subtraction
*   multiplication
/   division
```

Example:

```vesper
let result = 10 + 20 * 3;
print(result);
```

### Parentheses

```vesper
let result = (10 + 20) * 3;
print(result);
```

### Output

Use `print(...)`:

```vesper
print(42);
print(x);
print(x + 10);
```

---

## Roadmap

Vesper is being developed incrementally.

### Language

* [x] Integer literals
* [x] Identifiers
* [x] `let` declarations
* [x] Arithmetic expressions
* [x] Operator precedence
* [x] Parenthesized expressions
* [x] Variable references
* [x] `print(...)`
* [ ] Better parser error handling
* [ ] Booleans
* [ ] Comparison operators
* [ ] Logical operators
* [ ] `if` expressions/statements
* [ ] `while`
* [ ] Mutable variables
* [ ] Functions
* [ ] Function calls
* [ ] `return`
* [ ] Strings
* [ ] Arrays
* [ ] Structs
* [ ] Pattern matching
* [ ] Type system
* [ ] Type inference

### Compiler

The long-term goal is to move beyond interpretation:

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
Semantic Analysis
      │
      ▼
     IR
      │
      ▼
 Optimization
      │
      ▼
Code Generation
      │
      ▼
 Machine Code
```

Potential future targets include:

* RISC-V
* x86-64
* LLVM
* WebAssembly

---

## Why Vesper?

Vesper is primarily a **learning and experimentation project**.

The goal isn't simply to create another programming language. The project is an opportunity to understand how languages work internally:

* How source code becomes tokens
* How grammars become parsers
* How expressions become trees
* How ASTs represent program structure
* How interpreters execute programs
* How variables are represented at runtime
* How type systems work
* How intermediate representations work
* How compilers eventually produce machine code

The project is intentionally being built from the ground up rather than immediately relying on a complete compiler framework.

---

## Development Philosophy

Vesper is developed incrementally.

Rather than implementing the entire language at once, each feature is added through the complete pipeline:

```text
Language Feature
      ↓
Lexer
      ↓
Parser
      ↓
AST
      ↓
Interpreter
      ↓
Tests
```

This makes the implementation easier to understand and provides a foundation for eventually building a compiler.

---

## Contributing

Vesper is currently an experimental personal project, but ideas, issues, and discussions are welcome.

If you're interested in programming languages, compilers, interpreters, C++, or low-level systems programming, feel free to explore the code and open an issue.

---

## License

This project is licensed under the **GPL3 License**.
