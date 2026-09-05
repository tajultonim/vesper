---
layout: default
title: Roadmap
permalink: /roadmap/
---

**[Home](/) · [Installation](/installation/) · [Building & Testing](/building/) · [Variables](/variables/) · [Types](/types/) · [Arrays](/arrays/) · [Operators](/operators/) · [Functions](/functions/) · [Architecture](/architecture/) · [Roadmap](/roadmap/) · [Contributing](/contributing/)**

---

# Roadmap

Vesper is still in its early stages and is being developed incrementally.

## Language

- [x] Variables & mutable variables (`mut`)
- [x] Type annotations
- [x] Type inference
- [x] Primitive types
- [x] Arrays
- [x] Nested arrays
- [x] Array indexing
- [x] Arithmetic operators
- [x] Remainder (`%`)
- [x] Floor division (`//`)
- [x] Exponentiation (`**`)
- [x] Unary operators
- [x] String escape sequences
- [x] Multi-argument `print`
- [x] Comments
- [x] Static type checking
- [x] Source formatter
- [ ] Functions
- [ ] Function calls
- [ ] Explicit `return`
- [ ] `for` loops
- [ ] `range`
- [ ] Tuples
- [ ] Slicing
- [ ] Structs
- [ ] Pattern matching
- [ ] Generics
- [ ] Modules / imports
- [ ] Standard library

## Compiler

- [ ] Intermediate representation
- [ ] IR interpreter
- [ ] Constant folding
- [ ] Basic optimizations
- [ ] Native code generation
- [ ] C backend
- [ ] RISC-V backend
- [ ] LLVM backend
- [ ] AOT compilation
- [ ] JIT compilation
- [ ] C FFI / C ABI integration

## Scientific computing

The long-term goal is to make Vesper useful for numerical and scientific workloads. Planned areas include:

- [ ] Matrix types
- [ ] Tensor types
- [ ] Linear algebra
- [ ] Numerical methods
- [ ] Scientific standard library
- [ ] SIMD / vectorization
- [ ] Parallel computing
- [ ] MPI support
- [ ] GPU computing

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

See [Architecture](/architecture/) for how the current interpreter-first pipeline is meant to evolve toward this, and [Contributing](/contributing/) if you'd like to help move any of these items forward.
