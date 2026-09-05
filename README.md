<h1 align="center">
    <img src="https://github.com/tajultonim/vesper/blob/main/icons/vesper-animated.svg" width="70"/><br/>
    VESPER
</h1>

> A small, expressive programming language designed for learning, experimentation, and scientific computing.  
> For full details please read the wiki [here](https://github.com/tajultonim/vesper/wiki).

**Vesper** is a programming language I'm building from scratch in C++. The project started as an exploration of how programming languages work internally-from lexing and parsing to type checking and interpretation, with compilation and native code generation planned for the future.

The long-term goal is to evolve Vesper into a language particularly suited for **scientific computing and high-performance computing (HPC)** while keeping the language simple, expressive, and easy to understand.

## 📚 Project Status

**Vesper is experimental and under active development.**

The language syntax, type system, runtime, standard library, and compiler architecture may change substantially as development continues.

It is currently intended for **learning, experimentation, and language-development research**, rather than production use.


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

## 🚀 Getting started

```bash
git clone https://github.com/tajultonim/vesper.git
cd vesper

cmake -S . -B build
cmake --build build

./build/vesper examples/hello.vsp
```

## 📖 Documentation

Full documentation lives in the **[wiki](https://github.com/tajultonim/vesper/wiki)**:

- [Installation](https://github.com/tajultonim/vesper/wiki/Installation)
- [Building & Testing](https://github.com/tajultonim/vesper/wiki/Building-and-Testing)
- [Variables](https://github.com/tajultonim/vesper/wiki/Variables) · [Types](https://github.com/tajultonim/vesper/wiki/Types) · [Arrays](https://github.com/tajultonim/vesper/wiki/Arrays) · [Operators](https://github.com/tajultonim/vesper/wiki/Operators) · [Functions](https://github.com/tajultonim/vesper/wiki/Functions)
- [Architecture](https://github.com/tajultonim/vesper/wiki/Architecture)
- [Roadmap](https://github.com/tajultonim/vesper/wiki/Roadmap)
- [Contributing](https://github.com/tajultonim/vesper/wiki/Contributing)

## 🔨 Built with

C++17, CMake, and a hand-written lexer, recursive-descent parser, AST, static type checker, and tree-walking interpreter - plus `vspfmt`, Vesper's own source formatter, and CTest for automated testing.

## 📄 License

Vesper is released under the **GPL3 License**.

See [`LICENSE`](LICENSE) for details.

---

## 👤 Author

**Tajul Tonim**

Built as an ongoing exploration of programming languages, compilers, numerical computing, and high-performance computing.
