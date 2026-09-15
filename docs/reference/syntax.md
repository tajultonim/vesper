---
layout: default
title: Syntax
parent: Reference
nav_order: 1
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
       | "[" type "]"
```

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
function ::= "fn" identifier "(" parameter_list? ")"
             ":" type
             "{"
                 statement*
             "}"

parameter_list ::= parameter ("," parameter)*

parameter ::= identifier ":" type
              ("=" expression)?
```

## Return

```text
return_statement ::= "return" expression ";"
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
