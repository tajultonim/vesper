---
layout: default
title: Vesper
description: A small, expressive programming language for learning, experimentation, and scientific computing.
permalink: /
---

**[Home](/) · [Installation](/installation/) · [Building & Testing](/building/) · [Variables](/variables/) · [Types](/types/) · [Arrays](/arrays/) · [Operators](/operators/) · [Functions](/functions/) · [Architecture](/architecture/) · [Roadmap](/roadmap/) · [Contributing](/contributing/)**

---

# Vesper

> A small, expressive programming language designed for learning, experimentation, and scientific computing.

Vesper is a programming language built from scratch in C++17. It started as an exploration of how programming languages work internally — lexing, parsing, type checking, interpretation — with compilation and native code generation planned for the future.

The long-term goal is to grow Vesper into a language suited for **scientific computing and high-performance computing (HPC)**, while keeping the core language simple and easy to understand.

**Status:** experimental, under active development. Syntax, type system, runtime, and architecture may change substantially. Vesper is currently meant for learning and language-research, rather than production use.

---

## Quick taste

```vesper
let name = "Vesper";
mut counter = 0;

fn square(x: float): float {
    x ** 2
}

let matrix: [[int]] = [
    [1, 2, 3],
    [4, 5, 6]
];

print("Hello, ", name, "\n");
print(square(4.0), "\n");
print(matrix[0][1]);
```

Head to [Variables](/variables/), [Types](/types/), [Arrays](/arrays/), [Operators](/operators/), and [Functions](/functions/) for the full language tour, or jump straight to [Installation](/installation/) if you just want to run it.

---

## Where to go next

- **New here?** Start with [Installation](/installation/), then [Building & Testing](/building/).
- **Learning the language?** [Variables](/variables/) → [Types](/types/) → [Arrays](/arrays/) → [Operators](/operators/) → [Functions](/functions/), in that order.
- **Curious how it's built?** See [Architecture](/architecture/) for the current and planned pipeline.
- **Wondering what's next?** Check the [Roadmap](/roadmap/).
- **Want to help?** Read [Contributing](/contributing/).

---

## Built with

C++17, CMake, and a hand-written lexer, recursive-descent parser, AST, static type checker, and tree-walking interpreter — plus `vspfmt`, Vesper's own source formatter, and CTest for automated testing.

## License

Vesper is released under the [GPL-3.0 License](https://github.com/tajultonim/vesper/blob/main/LICENSE).

## Author

Built by [Tajul Tonim](https://github.com/tajultonim), as an ongoing exploration of programming languages, compilers, numerical computing, and high-performance computing.

[View the repository on GitHub →](https://github.com/tajultonim/vesper)
