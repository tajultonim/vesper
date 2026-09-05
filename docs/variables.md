---
layout: default
title: Variables
nav_order: 4
---

Variables in Vesper are **immutable by default**. If you need to reassign a variable after it's declared, mark it explicitly with `mut`.

```vesper
let name = "Vesper";
let age: int = 20;

mut counter = 0;
counter = counter + 1;
```

`let` bindings cannot be reassigned — attempting to assign to a `let` variable after its initial declaration is rejected. `mut` bindings can be reassigned freely, as long as the new value's type matches.

Variables can carry an explicit type annotation (`let age: int = 20;`) or have their type inferred from the initializer — see [Types](types.html) for the full picture of Vesper's type system, and [Arrays](arrays.html) for how variables hold array values.