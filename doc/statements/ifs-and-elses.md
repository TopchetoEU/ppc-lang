# Ifs and elses

## Syntax

Each if statement consists of the `if` keyword, followed by an condition expression in parenthesis, followed by an else body statement. Optionally, the statement may be followed by an `else` keyword, which is followed by a body statement on its own.

```c#
if (condition) body
// or
if (condition) body else else_body
```
No semicolons are required after the body of an if/else statement, unless the body statement itself requires one.

## Behavior

The condition may be of any integral type.

First, the condition is checked. If it's non-zero, the body is executed. If it's a zero (and there's an else body), the else body gets executed.

## Optimizations

The compiler may either 1. omit condition checking and the else body, and just keep the body, if the condition is determined to be non-zero, or 2. omit the condition check and the body, if the condition is determined to be zero.
