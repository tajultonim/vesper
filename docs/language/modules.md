---
layout: default
title: Modules
parent: Language
nav_order: 9
---

# Modules

Vesper modules are `.vsp` files that can be imported with an alias.

```vesper
import "math" as math;

let result = math.square(5.0);
```

## Relative resolution

Every import is resolved relative to the file containing the import. The
`.vsp` extension is optional, and `math` and `./math` both refer to a sibling
`math.vsp` when used from the same directory.

```text
project/
├── main.vsp
└── modules/
    ├── physics.vsp
    └── math.vsp
```

`modules/physics.vsp` can import its sibling either way:

```vesper
import "math" as math;
# or: import "./math" as math;
```

Nested imports use the same rule. A module imported by `main.vsp` can import
another module from its own directory without requiring `main.vsp` to import
that dependency separately.

## Caching and cycles

The loader caches modules by their resolved file path. Repeated imports of the
same file reuse the cached module. Circular dependencies are detected and
reported as module errors.

## Module functions

Functions declared in an imported module are accessed through the import alias:

```vesper
import "physics" as p;
let energy = p.energy(5.0, 3.0);
```
