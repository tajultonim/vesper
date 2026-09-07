---

layout: default
title: While Loop
parent: Language
nav_order: 7
------------

# While Loop

The `while` statement repeatedly executes a block of code as long as its condition evaluates to `true`.

## Syntax

```vesper
while (condition) {
    // code
}
```

The condition is evaluated before each iteration of the loop.

If the condition evaluates to `true`, the loop body is executed. After the body finishes, the condition is evaluated again.

When the condition evaluates to `false`, the loop terminates and execution continues with the statement following the loop.

## Example

```vesper
let mut i = 0

while (i < 5) {
    println(i)
    i = i + 1
}
```

Output:

```text
0
1
2
3
4
```

The loop executes while `i < 5`. Once `i` becomes `5`, the condition becomes `false` and the loop terminates.

## Infinite Loops

A `while` loop can run indefinitely if its condition never becomes `false`.

```vesper
while (true) {
    print("Running...")
}
```

## Nested While Loops

A `while` loop can contain another `while` loop.

```vesper
let mut i = 0

while (i < 3) {
    let mut j = 0

    while (j < 3) {
        println(i, j)
        j = j + 1
    }

    i = i + 1
}
```

## Loop Variable Scope

Variables declared inside the loop body are scoped to that block.

```vesper
while (true) {
    let x = 10
    println(x)
}

// x is not available here
```

Variables declared outside the loop can be accessed from within the loop, subject to Vesper's variable and mutability rules.

```vesper
let mut counter = 0

while (counter < 10) {
    counter = counter + 1
}

println(counter)
```

Output:

```text
10
```

## Common Patterns

### Counting

```vesper
let mut i = 0

while (i < 10) {
    println(i)
    i = i + 1
}
```

## Summary

The basic syntax of a Vesper `while` loop is:

```vesper
while (condition) {
    // code
}
```

A `while` loop:

1. Evaluates the condition.
2. Executes the body if the condition is `true`.
3. Evaluates the condition again.
4. Repeats until the condition becomes `false`.