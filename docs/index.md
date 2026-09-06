---
layout: default
title: Home
nav_order: 1
---

<h1 align="center">
    <img src="https://raw.githubusercontent.com/tajultonim/vesper/refs/heads/main/icons/vesper-animated.svg" width="90"/><br/>
    VESPER
</h1>

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

For the full language tour, see [Variables](variables.html), [Types](types.html), [Arrays](arrays.html), [Operators](operators.html), and [Functions](functions.html). To just run it, start with [Installation](installation.html).

## Where to go next

- **New here?** Start with [Installation](installation.html), then [Building and Testing](building-and-testing.html).
- **Learning the language?** [Variables](variables.html) → [Types](types.html) → [Arrays](arrays.html) → [Operators](operators.html) → [Functions](functions.html), in that order.
- **Curious how it's built?** See [Architecture](architecture.html) for the current and planned pipeline.
- **Wondering what's next?** Check the [Roadmap](roadmap.html).
- **Want to help?** Read [Contributing](contributing.html).

## Built with

C++17, CMake, and a hand-written lexer, recursive-descent parser, AST, static type checker, and tree-walking interpreter — plus `vspfmt`, Vesper's own source formatter, and CTest for automated testing.

## License

Vesper is released under the [GPL-3.0 License](https://github.com/tajultonim/vesper/blob/main/LICENSE).

## Author

Built by [Tajul Tonim](https://github.com/tajultonim), as an ongoing exploration of programming languages, compilers, numerical computing, and high-performance computing.