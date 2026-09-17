---
layout: default
title: Syntax
parent: Reference
nav_order: 1
description: "Formal reference for Vesper declarations, statements, types, expressions, imports, calls, arrays, and member access."
---

# Syntax Reference

This page summarizes the currently implemented syntax.

## Program

```text
program ::= statement*
```

## Variable declarations

```text
declaration ::= "let" identifier (":" type)? "=" expression ";"
              | "mut" identifier (":" type)? "=" expression ";"
```

## Assignment

```text
assignment ::= identifier "=" expression ";"
```

## Types

```text
type ::= "int"
       | "float"
       | "bool"
       | "string"
             | "void"
       | "[" type "]"
```

## Imports

```text
import_statement ::= "import" string "as" identifier ";"
```

An import is resolved relative to the file that contains it. The `.vsp`
extension is optional, and `math` and `./math` both resolve to a sibling
`math.vsp` when written inside the same directory.

## If statement

```text
if_statement ::= "if" "(" expression ")" "{"
                     statement*
                 "}"
                 ("else" "{"
                     statement*
                 "}")?
```

## While statement

```text
while_statement ::= "while" "(" expression ")" "{"
                        statement*
                    "}"
```

## Function

```text
function ::= ("export")? ("extern")? "fn" identifier "(" parameter_list? ")"
             ":" type (";" | "{" statement* "}")

parameter_list ::= parameter ("," parameter)*

parameter ::= identifier ":" type
              ("=" expression)?
```

## Return

```text
return_statement ::= "return" expression? ";"
```

## Expressions

Conceptually, expression precedence is:

```text
expression
    → comparison
        → addition
            → multiplication
                → unary
                    → exponentiation
                        → postfix
                            → primary
```

Postfix expressions include calls and indexing.

## Primary expressions

```text
primary ::= integer
          | float
          | boolean
          | string
          | identifier
          | array
          | "(" expression ")"
```

## Array

```text
array ::= "[" (expression ("," expression)*)? "]"
```

## Function call

```text
call ::= expression "(" (expression ("," expression)*)? ")"
```

## Index

```text
index ::= expression "[" expression "]"
```

## Member access

```text
member ::= expression "." identifier
```

Module functions can be called through an imported alias:

```vesper
import "math" as m;
let result = m.square(5.0);
```
