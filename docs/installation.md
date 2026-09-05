---
layout: default
title: Installation
permalink: /installation/
---

**[Home](/) · [Installation](/installation/) · [Building & Testing](/building/) · [Variables](/variables/) · [Types](/types/) · [Arrays](/arrays/) · [Operators](/operators/) · [Functions](/functions/) · [Architecture](/architecture/) · [Roadmap](/roadmap/) · [Contributing](/contributing/)**

---

# Installation

Vesper is not yet distributed as a pre-built binary or package — you build it from source. This page covers what you need before building; see [Building & Testing](/building/) for the actual build steps.

## Prerequisites

Vesper is implemented in C++17 and built with CMake, so you'll need:

- **A C++17-capable compiler** — GCC or MinGW are used during development; any recent GCC, Clang, or MSVC that supports C++17 should work.
- **CMake** (a recent 3.x release).
- **Git**, to clone the repository.
- **CTest**, for running the test suite (this ships with CMake, so no separate install is needed).

## Getting the source

```bash
git clone https://github.com/tajultonim/vesper.git
cd vesper
```

There are no external library dependencies to install — the lexer, parser, AST, type checker, and interpreter are all implemented directly in the `src/` tree.

## Next step

Once you have the prerequisites and the source, head to [Building & Testing](/building/) to compile Vesper and run your first program.
