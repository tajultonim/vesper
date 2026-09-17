---
layout: default
title: VS Code Extension
parent: Tools
nav_order: 3
description: "Set up the Vesper VS Code extension with syntax highlighting, autocomplete, formatting, and a Run button."
---

# VS Code Extension

Install the [Vesper VS Code extension from the Marketplace](https://marketplace.visualstudio.com/items?itemName=tajultonim.vesper-vscode).

The Vesper VS Code extension supports `.vsp` files with:

- syntax highlighting
- autocomplete for keywords, types, builtins, variables, and functions
- document formatting through `vspfmt`
- a Run command and editor-title Run button
- Vesper file icons

The extension package does not include `vesper.exe` or `vspfmt.exe`.

## Interpreter selection

The Run command checks these locations in order:

1. `vesper.interpreterPath`, when configured
2. `bin/vesper.exe` inside the extension, if present
3. `build/vesper.exe` in an open workspace
4. `vesper.exe` in the workspace root
5. the `vesper` command from `PATH`

Configure a custom path in VS Code settings:

```json
{
    "vesper.interpreterPath": "C:/path/to/vesper.exe"
}
```

## Formatter selection

Formatting uses the same pattern through `vesper.formatterPath`, followed by
workspace builds and the `vspfmt` command from `PATH`.

```json
{
    "vesper.formatterPath": "C:/path/to/vspfmt.exe"
}
```

The extension formats the current editor content, including unsaved changes.
