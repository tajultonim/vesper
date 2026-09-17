---
layout: default
title: Comments
parent: Language
nav_order: 1
description: "Use line comments and block comments in Vesper source code."
---

# Comments

Vesper supports single-line and block comments.

## Single-line comments

A single-line comment begins with `#`.

```vesper
# This is a comment

let x: int = 10; # This is also a comment
```

Everything after `#` on the line is part of the comment.

## Block comments

Block comments use `##` as both the opening and closing delimiter.

```vesper
##
This is a block comment.
It can span multiple lines.
##
```

## Syntax

```text
# comment

## comment
   comment
##
```

Comments do not affect program execution.
