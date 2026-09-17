---
layout: default
title: Installation
nav_order: 2
description: "Install Vesper from source or release binaries, configure PATH, and set up the Vesper VS Code extension."
---

# Installation

Vesper provides an interpreter named `vesper` and a formatter named `vspfmt`.
Both programs are built from this repository. Prebuilt files may also be
available on the [latest GitHub release](https://github.com/tajultonim/vesper/releases/latest).

## Build from source

### Requirements

- CMake 3.20 or newer
- A C++17 compiler
- Git

Clone the repository and configure a build directory:

```bash
git clone https://github.com/tajultonim/vesper.git
cd vesper
cmake -S . -B build
cmake --build build
```

The resulting executables are located in `build/`:

```text
build/vesper
build/vspfmt
```

On Windows, the files have an `.exe` extension:

```text
build/vesper.exe
build/vspfmt.exe
```

## Add Vesper to PATH

Adding the build directory to `PATH` lets you run both commands from any
terminal.

### PowerShell

Replace the path with the absolute path to your Vesper `build` directory:

```powershell
$env:Path += ";C:\path\to\vesper\build"
```

This changes the current terminal session. To set it permanently, add the
same directory through **System Properties > Environment Variables > Path**.

### Bash or Zsh

```bash
export PATH="$PATH:/path/to/vesper/build"
```

Add that line to `~/.bashrc` or `~/.zshrc` for future sessions.

Check the installation:

```text
vesper --version
vspfmt --help
```

## Use without PATH

You can invoke the executables directly:

```text
build/vesper examples/hello.vsp
build/vspfmt examples/hello.vsp
```

On Windows PowerShell:

```powershell
.\build\vesper.exe .\examples\hello.vsp
.\build\vspfmt.exe .\examples\hello.vsp
```

## VS Code

For syntax highlighting, autocomplete, formatting, and a Run button, install
the [Vesper VS Code extension from the Marketplace](https://marketplace.visualstudio.com/items?itemName=tajultonim.vesper-vscode).

The extension does not include the Vesper binaries. It can use workspace builds,
commands available on `PATH`, or paths configured through
`vesper.interpreterPath` and `vesper.formatterPath`.

See [VS Code Extension](tools/vscode.md) for the lookup order and settings.

## Next steps

Continue to [Getting Started](getting-started.md) to run a program, use the
formatter, and learn the development pipeline.
