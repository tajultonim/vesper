---
layout: default
title: Building and Testing
nav_order: 3
---

## Building

With the repository cloned (see [Installation](installation.html)), configure and build with CMake:

```bash
cmake -S . -B build
cmake --build build
```

This produces the `vesper` interpreter binary, along with `vspfmt`, Vesper's source formatter, inside the `build/` directory.

## Running a program

```bash
./build/vesper examples/hello.vsp
```

On Windows:

```bash
.\build\vesper.exe .\examples\hello.vsp
```

The `examples/` directory contains sample `.vsp` source files you can run or use as a starting point.

## Formatting source with `vspfmt`

`vspfmt` reformats Vesper source based on its parsed program structure — not just text manipulation — so it correctly handles expressions, indentation, operators, arrays, indexing, and nested statements.

Print the formatted result to stdout:

```bash
./build/vspfmt ./examples/hello.vsp
```

Format and write the result back to the file in place:

```bash
./build/vspfmt ./examples/hello.vsp -w
```

## Testing

Vesper uses CTest for its test suite, which currently covers static type-checking (`tests/type_checker_test.cpp`).

Run the tests after building:

```bash
ctest --test-dir build
```

Or build and test in one go:

```bash
cmake --build build
ctest --test-dir build
```

If you're contributing changes, running the test suite — and formatting any `.vsp` examples you touch with `vspfmt` — before opening a pull request is a good habit. See [Contributing](contributing.html) for the full workflow.