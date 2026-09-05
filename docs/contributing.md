---
layout: default
title: Contributing
permalink: /contributing/
---

**[Home](/) · [Installation](/installation/) · [Building & Testing](/building/) · [Variables](/variables/) · [Types](/types/) · [Arrays](/arrays/) · [Operators](/operators/) · [Functions](/functions/) · [Architecture](/architecture/) · [Roadmap](/roadmap/) · [Contributing](/contributing/)**

---

# Contributing

Vesper is an experimental, early-stage language, built as a learning project. Contributions are welcome, but because the language, type system, and architecture are still actively shifting (see the [Roadmap](/roadmap/)), it's worth checking in before investing time in anything large.

## Before you start

- **Small fixes** (typos, bugs, small formatter or lexer edge cases): feel free to open a pull request directly.
- **Larger changes** (new syntax, new types, changes to the interpreter or type checker, compiler work): open an issue first to discuss the approach. This avoids duplicated effort and keeps changes aligned with where the [Architecture](/architecture/) and [Roadmap](/roadmap/) are heading.

## Development setup

Follow [Installation](/installation/) and [Building & Testing](/building/) to get a working build:

```bash
git clone https://github.com/tajultonim/vesper.git
cd vesper

cmake -S . -B build
cmake --build build
ctest --test-dir build
```

## Making a change

1. **Fork the repository** and create a branch off `main` for your change.
2. **Write your change**, along with tests where applicable. The existing test suite (`tests/type_checker_test.cpp`, run via CTest) is the model to follow for type-checker changes — add cases alongside it rather than starting a new ad hoc test file.
3. **Match the existing code style.** The repository includes a `.clang-format` file — run your changes through `clang-format` before committing so C++ formatting stays consistent.
4. **Format any `.vsp` example files you touch** with Vesper's own formatter:
   ```bash
   ./build/vspfmt path/to/example.vsp -w
   ```
5. **Run the test suite** and make sure it passes:
   ```bash
   cmake --build build
   ctest --test-dir build
   ```
6. **Keep commits focused.** A commit (or a PR) should represent one coherent change — a new lexer token, a fix to floor-division semantics, a new example program — rather than bundling unrelated work together.

## Opening a pull request

- Describe **what** changed and **why**, not just how.
- Reference the related issue if there is one.
- If the change affects language behavior (new syntax, changed semantics, new error cases), include a short example `.vsp` snippet in the PR description showing the before/after behavior.
- Be ready for feedback — as a young, single-maintainer project, review may take a little time, and requested changes are part of getting things right while the design is still settling.

## Reporting bugs

When filing an issue, include:

- The Vesper source that triggers the problem (a minimal `.vsp` snippet is ideal).
- What you expected to happen versus what actually happened.
- Your platform and compiler (e.g. "Ubuntu 22.04, GCC 12" or "Windows 11, MinGW").
- The output of `cmake --build build` if the issue is a build failure.

## Proposing language features

Since Vesper's design is still taking shape, feature proposals are especially useful as issues rather than surprise pull requests. A good proposal covers:

- The problem the feature solves, ideally with a motivating example.
- Proposed syntax, shown as `.vsp` code.
- How it interacts with the existing type system (see [Types](/types/)) — does it introduce a new type, a new operator, new inference rules?
- Where it fits on the [Roadmap](/roadmap/) — is this filling in an existing planned item, or something new?

## Code of conduct

There isn't a separate code of conduct document yet — in the meantime, the expectation is the usual open-source baseline: be respectful in issues and reviews, assume good faith, and keep disagreements about the code, not the person.

## License

By contributing, you agree that your contributions will be licensed under the project's [GPL-3.0 License](https://github.com/tajultonim/vesper/blob/main/LICENSE).
