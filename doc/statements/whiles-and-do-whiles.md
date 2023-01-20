# While loops

## Syntax

A while loop starts with the `while` keyword. After it, the condition is put in parentheses, and then the while statement ends with a body statement. The while statement doesn't require an ending semicolon, if the body statement doesn't require one.

```c
while (expression) statement
```

## Behavior

The condition may be of any integral type.

The execution of the loop will start from checking whether or not the condition is non-zero. If so, The body of the loop will be executed, and the check will be performed again. This will be done, until the condition becomes zero. If the condition is initially zero, the body won't get executed.

## Optimizations

The compiler may either 1. omit the while loop if the condition is determined to be a zero, or 2. omit the condition check and everything after the loop if the condition is determined to be non-zero.


# Do-while loops

## Syntax

A while loop starts with the `do` keyword, followed by the body statement. After it, there is a `while` keyword, followed by the condition, in parentheses, followed by a semicolon.

```c
do statement while (expression);
```

## Behavior

The condition may be of any integral type.

The execution of the loop will start from executing the body. Then, if the condition is non-zero, the whole process will repeat. If not, execution continues after the loop.

## Optimizations

The compiler may either 1. omit condition checking, and just keep the body, if the condition is determined to be a zero, or 2. omit the condition check and everything after the loop if the condition is determined to be non-zero.
