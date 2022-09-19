# Ifs and elses

## Syntax

Each if statement consists of the `if` keyword, followed by an expression, encapsulated in parenthesis, followed by a statement. Optionally, the statement may be followed by an `else` keyword, which is followed by a statement on its own.

```c#
if (expression) statement
// or
if (expression) statement1 else statement2
```

No semicolons are required after the body of an if statement, unless the body statement itself requires one.

## Terminology

The given expression is called a `condition`, the statement following the condition is the `if body` and the statement following the else keyword is called the `else body`

## Behavior

The given expression (condition) may be of any type, and if its non-zero, then the if body will be executed. Else, if one, the else body will be executed instead.

## Equivalent PPCIL assembly

For any given if-else statement:

```c
if (condition) if_body
else else_body
```

Its equivalent assembly is:

```
;condition
jz else
; if_body
jmp end_else
else:
;else_body
end_else:
```

If the if doesn't have a corresponding else statement, the resulting assembly is as follows:

```
;condition
jz else
; if_body
else:
```


## Possible compiler optimizations

An if body may be omitted if the condition is determined to be `false`. If the condition is determined to be `true`, then the else body is omitted.
