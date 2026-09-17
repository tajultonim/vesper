---
layout: default
title: Command Line
parent: Tools
nav_order: 1
---

# Command Line

The Vesper executable is invoked with a `.vsp` source file.

```text
vesper <file.vsp>
```

## Help

```text
vesper --help
vesper -h
```

## Version

```text
vesper --version
vesper -v
```

The version is reported by the executable:

```text
vesper --version
```

## Module paths

Imports are resolved relative to the importing file. These declarations are
equivalent when `physics.vsp` and `math.vsp` are siblings:

```vesper
import "math" as math;
import "./math" as math;
```

The loader caches each resolved module and detects circular dependencies.
